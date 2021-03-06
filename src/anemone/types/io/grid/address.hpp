/**
 * @file   types/io/grid/address.hpp
 * @brief  IO Grid Address Types
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef TYPES_IO_GRID_LAYOUT_ADDRESS_H
#define TYPES_IO_GRID_LAYOUT_ADDRESS_H

#include "anemone/types/io/grid/layout/names.hpp"


/// @brief type alias for grid sectionn index.
typedef unsigned int grid_section_index_t;

/// @breif grid address.
struct grid_addr_t {
  LayoutName           layout;
  GridSectionName      section;
  grid_section_index_t index;

  /// @brief grid address equality operator.
  ///
  /// @remark
  /// Providing an equality (`==`) operator definition is necessary for using this
  /// struct as a key in a std lib map collection.
  ///
  bool operator==(const grid_addr_t &m) const {
    return layout == m.layout && section == m.section && index == m.index;
  };

};

/// @brief a grid address hasher functor.
///
/// @remark
/// In order to use `grid_addr_t` as a std lib unnordered map key, it is necessary
/// to provide a hasher function to the map constructor.
///
struct grid_addr_hasher {
  /// @brief override `()` operator for hash function.
  size_t operator()(const grid_addr_t &m) const
  {
    size_t h1 = std::hash<LayoutName>()(m.layout);
    size_t h2 = std::hash<GridSectionName>()(m.section);
    size_t h3 = std::hash<grid_section_index_t>()(m.index);
    return h1 ^ (h2 ^ (h3 << 1));
  };
};


#endif
