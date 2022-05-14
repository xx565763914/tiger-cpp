#pragma once

#include "model/market.hpp"
#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

class BaseBar {
  enum TYPE {
    Bar_Invalid = 0,
    Time_Bar,   // 等时抽样
    Tick_Bar,   // 等笔抽样
    Volume_Bar, // 等量抽样
    Amount_Bar, // 等额抽样
  };

public:
  BaseBar() = delete;
  BaseBar(Tick tick, BaseBar::TYPE type) : _type(type) {
    _ticks.clear();
    _ticks.resize(5);
    append(tick); // one tick at last
  }

  void append(Tick tick) { _ticks.emplace_back(std::move(tick)); };

  double open_price() { return _close; };
  double high_price() { return _high; };
  double low_price() { return _low; };
  double close_price() { return _close; };
  int volume() { return _volume; };

  std::string get_instrument_id() { return _ticks.begin()->instrument_id; };

  virtual bool finish_criteria() = 0;

private:
  virtual void generate_bar() {
    if (finish_criteria()) {
      auto iter = _ticks.begin();
      _open = iter->open_price;

      for (; _ticks.end() != iter; ++iter) {
        _volume += iter->volume; // lervisnhTODO : to deal vol
        _high = std::max(iter->highest_price, _high);
        _low = std::min(iter->lowest_price, _low);
      }

      _close = _ticks.back().close_price;
    }
  };

private:
  TYPE _type; // debug & check to use
  std::vector<Tick> _ticks;

  // price
  double _close = 0;
  double _open = 0;
  double _high = 0;
  double _low = 0;

  int _volume = 0;
};