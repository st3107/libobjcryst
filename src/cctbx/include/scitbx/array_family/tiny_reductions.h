/* *****************************************************
   THIS IS AN AUTOMATICALLY GENERATED FILE. DO NOT EDIT.
   *****************************************************

   Generated by:
     scitbx.source_generators.array_family.generate_reductions
 */

#ifndef SCITBX_ARRAY_FAMILY_TINY_REDUCTIONS_H
#define SCITBX_ARRAY_FAMILY_TINY_REDUCTIONS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <scitbx/array_family/ref_reductions.h>
#include <scitbx/array_family/tiny_plain.h>

namespace scitbx { namespace af {

  template <typename ElementType1, std::size_t N1,
            typename ElementType2, std::size_t N2>
  int
  inline
  order(
    tiny_plain<ElementType1, N1> const& a1,
    tiny_plain<ElementType2, N2> const& a2)
  {
    return order(a1.const_ref(), a2.const_ref());
  }

  template <typename ElementType, std::size_t N>
  inline
  std::size_t
  max_index(tiny_plain<ElementType, N> const& a)
  {
    return max_index(a.const_ref());
  }

  template <typename ElementType, std::size_t N>
  inline
  std::size_t
  min_index(tiny_plain<ElementType, N> const& a)
  {
    return min_index(a.const_ref());
  }

  template <typename ElementType, std::size_t N>
  inline
  ElementType
  max(tiny_plain<ElementType, N> const& a)
  {
    return max(a.const_ref());
  }

  template <typename ElementType, std::size_t N>
  inline
  ElementType
  min(tiny_plain<ElementType, N> const& a)
  {
    return min(a.const_ref());
  }

  template <typename ElementType, std::size_t N>
  inline
  ElementType
  max_absolute(tiny_plain<ElementType, N> const& a)
  {
    return max_absolute(a.const_ref());
  }

  template <typename ElementType, std::size_t N>
  inline
  ElementType
  sum(tiny_plain<ElementType, N> const& a)
  {
    return sum(a.const_ref());
  }

  template <typename ElementType, std::size_t N>
  inline
  ElementType
  sum_sq(tiny_plain<ElementType, N> const& a)
  {
    return sum_sq(a.const_ref());
  }

  template <typename ElementType, std::size_t N>
  inline
  ElementType
  product(tiny_plain<ElementType, N> const& a)
  {
    return product(a.const_ref());
  }

  template <typename ElementType, std::size_t N>
  inline
  ElementType
  mean(tiny_plain<ElementType, N> const& a)
  {
    return mean(a.const_ref());
  }

  template <typename ElementType, std::size_t N>
  inline
  ElementType
  mean_sq(tiny_plain<ElementType, N> const& a)
  {
    return mean_sq(a.const_ref());
  }

  template <typename ElementTypeValues, std::size_t N,
            typename ElementTypeWeights>
  inline
  ElementTypeValues
  mean_weighted(
    tiny_plain<ElementTypeValues, N> const& values,
    tiny_plain<ElementTypeWeights, N> const& weights)
  {
    return mean_weighted(values.const_ref(), weights.const_ref());
  }

  template <typename ElementTypeValues, std::size_t N,
            typename ElementTypeWeights>
  inline
  ElementTypeValues
  mean_sq_weighted(
    tiny_plain<ElementTypeValues, N> const& values,
    tiny_plain<ElementTypeWeights, N> const& weights)
  {
    return mean_sq_weighted(values.const_ref(), weights.const_ref());
  }

  template <typename ElementType, std::size_t N, class PredicateType>
  inline
  boost::optional<std::size_t>
  first_index(tiny_plain<ElementType, N> const& a,
               PredicateType p)
  {
    return first_index(a.const_ref(), p);
  }

  template <typename ElementType, std::size_t N, class PredicateType>
  inline
  boost::optional<std::size_t>
  last_index(tiny_plain<ElementType, N> const& a,
               PredicateType p)
  {
    return last_index(a.const_ref(), p);
  }

}} // namespace scitbx::af

#endif // DOXYGEN_SHOULD_SKIP_THIS

#endif // SCITBX_ARRAY_FAMILY_TINY_REDUCTIONS_H

