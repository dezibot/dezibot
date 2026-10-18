#pragma once

template <typename T> class AbstractSet {
public:
  virtual void insert(T item) = 0;
  virtual T pick() = 0;
  virtual bool isEmpty() const = 0;
  virtual ~AbstractSet() = default;
};
