#include "composite-shape.hpp"
#include "figureactions.hpp"
#include <cmath>
#include <stdexcept>

namespace
{
  void fill_array_with_clones(tkach::Shape** now, const tkach::Shape** other, const size_t size, size_t& true_size)
  {
    for (size_t i = 0; i < size; ++i)
    {
      now[i] = other[i]->clone();
      true_size++;
    }
  }
}

tkach::CompositeShape::CompositeShape(CompositeShape&& other) noexcept:
  size_(other.size_),
  shapes_(other.shapes_)
{
  other.size_ = 0;
  other.shapes_ = nullptr;
}

tkach::CompositeShape::CompositeShape(const CompositeShape& other):
  size_(other.size_),
  shapes_(new Shape*[size_])
{
  size_t true_size = 0;
  try
  {
    fill_array_with_clones(shapes_, other.shapes_, size_, true_size);
  }
  catch (const std::bad_alloc& e)
  {
    size_ = true_size;
    throw;
  }
}

tkach::CompositeShape& tkach::CompositeShape::operator=(const CompositeShape& other)
{
  if (this != &other)
  {
    Shape** new_shapes = new Shape*[other.size_];
    size_t true_size = 0;
    try
    {
      fill_array_with_clones(new_shapes, other.shapes_, other.size_, true_size);
    }
    catch (const std::bad_alloc& e)
    {
      for (size_t i = 0; i < true_size; ++i)
      {
        delete new_shapes[i];
      }
      delete[] new_shapes;
      throw;
    }
    delete[] shapes_;
    shapes_ = new_shapes;
    size_ = other.size_;
  }
  return *this;
}

tkach::CompositeShape& tkach::CompositeShape::operator=(CompositeShape&& other) noexcept
{
  if (this != &other)
  {
    size_ = other.size_;
    shapes_ = other.shapes_;
    other.shapes_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}
  
double tkach::CompositeShape::getArea() const
{
  return getTotalArea(shapes_, size_);
}

tkach::rectangle_t tkach::CompositeShape::getFrameRect() const
{
  if (size_ != 0)
  {
    return {0.0, 0.0, {0.0, 0.0}};
  }
  double left = 0.0, right = 0.0, top = 0.0, bot = 0.0;
  rectangle_t shape_rect = shapes_[0]->getFrameRect();
  left = shape_rect.pos.x - shape_rect.width / 2.0;
  bot = shape_rect.pos.y - shape_rect.height / 2.0;
  for (size_t i = 1; i < size_; ++i)
  {
    shape_rect = shapes_[i]->getFrameRect();
    left = std::min(left, shape_rect.pos.x - shape_rect.width / 2.0);
    bot = std::min(bot, shape_rect.pos.y - shape_rect.height / 2.0);
    right = std::max(right, shape_rect.pos.x + shape_rect.width / 2.0);
    top = std::max(top, shape_rect.pos.y + shape_rect.height / 2.0);
  }
  return {right - left, top - left, {(left + right) / 2.0, (top + bot) / 2.0}};
}

tkach::CompositeShape::~CompositeShape()
{
  delete[] shapes_;
}

void tkach::CompositeShape::move(double add_to_x, double add_to_y) noexcept
{
  for (size_t i = 0; i < size_; ++i)
  {
    shapes_[i]->move(add_to_x, add_to_y);
  }
}

void tkach::CompositeShape::move(const point_t& point_to_move) noexcept
{
  point_t frame_center = getFrameRect().pos;
  move(frame_center.x - point_to_move.x, frame_center.y - point_to_move.y);
}

void tkach::CompositeShape::push_back(Shape* shp)
{
  if (shp == nullptr)
  {
    throw std::invalid_argument("No shape");
  }
  Shape** new_shapes = new Shape*[size_ + 1];
  for (size_t i = 0; i < size_; ++i)
  {
    new_shapes[i] = shapes_[i];
  }
  new_shapes[size_++] = shp;
  delete[] shapes_;
  shapes_ = new_shapes;
}

void tkach::CompositeShape::pop_back()
{
  if (size_ == 0)
  {
    throw std::logic_error("Empty compositeshape");
  }
  Shape** new_shapes = new Shape*[--size_];
  for (size_t i = 0; i < size_; ++i)
  {
    new_shapes[i] = shapes_[i];
  }
  delete[] shapes_;
  shapes_ = new_shapes;
}

tkach::Shape* tkach::CompositeShape::at(size_t id)
{
  if (id >= size_)
  {
    throw std::out_of_range("Index is out of range");
  }
  return shapes_[id];
}

const tkach::Shape* tkach::CompositeShape::at(size_t id) const
{
  if (id >= size_)
  {
    throw std::out_of_range("Index is out of range");
  }
  return shapes_[id];
}

tkach::Shape* tkach::CompositeShape::operator[](size_t id)
{
  return shapes_[id];
}

const tkach::Shape* tkach::CompositeShape::operator[](size_t id) const
{
  return shapes_[id];
}

bool tkach::CompositeShape::empty() const noexcept
{
  if (size_ == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

size_t tkach::CompositeShape::size() const noexcept
{
  return size_;
}

void tkach::CompositeShape::scale(double multiplier)
{
  doIsoScaleShapes(shapes_, size_, multiplier, getFrameRect().pos);
}