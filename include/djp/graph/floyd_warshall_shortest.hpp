//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_GRAPH_FLOYD_WARSHALL_SHORTEST_HPP
#define DJP_GRAPH_FLOYD_WARSHALL_SHORTEST_HPP

#include <djp/utility/matrix.hpp>
#include <algorithm> // For std::min
#include <limits>    // For std::numeric_limits
#include <vector>    // For std::vector
#include <cstddef>   // For std::size_t
#include <cstdint>   // For SIZE_MAX

namespace djp {

/// \brief Finds the shortest-distance for each pair of vertices in a directed
/// edge-weighted graph.
///
/// This algorithm should be used to compute all-pairs shortest paths in a dense
/// graph. If the target graph is sparse, the Johnson's algorithm should be
/// used instead.
///
/// \param g The target graph.
///
/// \param weight Edge-weight map.
///
/// \param[out] dist Vertex-distance matrix. The shortest distance between each
/// pair of vertices <tt>(u, v)</tt> is stored at location <tt>dist[{u,
/// v}]</tt>. If no path exist from \p u to \p v, <tt>dist[{u, v}]</tt> is set
/// to <tt>std::numeric_limits<Distance>::max()</tt>.
///
/// \param[out] next Auxiliary matrix used to reconstruct paths, it can be used
/// as input in the \c floyd_warshall_path function.
///
/// \pre The graph \p g must have no negative cycle.
///
/// \par Complexity
/// The time complexity is <tt>O(V^3)</tt>.
///
template <typename Graph, typename Distance>
void floyd_warshall_all_pairs_shortest_paths(
    const Graph &g, const std::vector<Distance> &weight, matrix<Distance> &dist,
    matrix<size_t> &next) {

  const Distance inf = std::numeric_limits<Distance>::max();
  const size_t num_edges = g.num_edges();
  const size_t num_v = g.num_vertices();
  dist.assign({num_v, num_v}, inf);
  next.assign({num_v, num_v}, SIZE_MAX);

  for (size_t v = 0; v != num_v; ++v)
    dist[{v, v}] = 0;

  for (size_t e = 0; e != num_edges; ++e) {
    const size_t u = g.source(e);
    const size_t v = g.target(e);
    dist[{u, v}] = std::min(dist[{u, v}], weight[e]);
    next[{u, v}] = v;
  }

  for (size_t k = 0; k != num_v; ++k)
    for (size_t i = 0; i != num_v; ++i)
      if (dist[{i, k}] != inf)
        for (size_t j = 0; j != num_v; ++j)
          if (dist[{k, j}] != inf)
            if (dist[{i, k}] + dist[{k, j}] < dist[{i, j}]) {
              dist[{i, j}] = dist[{i, k}] + dist[{k, j}];
              next[{i, j}] = next[{i, k}];
            }
}

/// \brief Reconstruct the shortest path between two vertices using the data
/// generated by the Floyd-Warshall algorithm.
///
/// \param u The source vertex.
/// \param v The target vertex.
/// \param next Auxiliary matrix used to reconstruct paths. It must be generated
/// by the \c floyd_warshall_all_pairs_shortest_paths function.
///
/// \param out_it Beginning of the destination range. Descriptors of visited
/// vertices will be copied to this range (ordered by visit time).
///
/// \returns Iterator to element in the destination range, one past the last
/// element copied.
///
/// \par Complexity
/// Linear in the number of vertices on the reconstructed path.
///
template <typename OutputIt>
OutputIt floyd_warshall_path(size_t u, const size_t v,
                             const matrix<size_t> &next, OutputIt out_it) {
  if (u != v && next[{u, v}] == SIZE_MAX)
    return out_it;
  *out_it++ = u;
  while (u != v) {
    u = next[{u, v}];
    *out_it++ = u;
  }
  return out_it;
}

} // end namespace djp

#endif // Header guard
