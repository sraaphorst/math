/**
 * AbstractSimplicialComplex.h
 *
 * By Sebastian Raaphorst, 2019.
 */

#pragma once

#include <set>

namespace vorpal::algebra {
    /**
     * This is an immutable abstract simplicial complex (ASC).
     * It is defined by its facets, which are the maximal edges.
     * The total edge set comprises all non-empty subsets of the facets.
     * The vertex set comprises the union of all the vertices in the facets.
     * We only calculate the vertex set and the edge set upon demand; only the facets are stored.
     */
    class AbstractSimplicialComplex final {
    public:
        using vertex_t   = int;
        using vertices_t = std::set<int>;
        using edge_t     = std::set<int>;
        using edges_t    = std::set<edge_t>;

    private:
        // An ASC is definted fully by its facets.
        const edges_t facets{};

        /**
         * In order to maintain the integrity of the facet set, i.e. that no set is a subset of another,
         * we force the sets to be added one-at-a-time through the create methods below. This is only used
         * internally with sets that we can guarantee are subset-free.
         */
        explicit AbstractSimplicialComplex(edges_t) noexcept;

    public:
        AbstractSimplicialComplex() noexcept = default;
        AbstractSimplicialComplex(const AbstractSimplicialComplex&) = default;
        AbstractSimplicialComplex(AbstractSimplicialComplex&&) = default;
        ~AbstractSimplicialComplex() noexcept = default;

        bool operator==(const AbstractSimplicialComplex&) const noexcept;
        bool operator!=(const AbstractSimplicialComplex&) const noexcept;

        /**
         * Add an edge to an ASC. Analagous to addEdges with a single edge.
         * @return the new ASC
         */
        AbstractSimplicialComplex operator+(const edge_t&) const noexcept;

        template<typename E>
        AbstractSimplicialComplex addEdges(E e) {
            return *this + e;
        }

        template<typename E, typename... Edges>
        AbstractSimplicialComplex addEdges(E e, Edges... es) {
            return (*this + e).addEdges(es...);
        }

        /**
         * Create an ASC from a single facet, if specified.
         */
        template<typename E>
        static AbstractSimplicialComplex create(E e) {
            if (e.empty())
                return AbstractSimplicialComplex{};
            else
                return AbstractSimplicialComplex{edges_t{e}};
        }

        /**
         * Create an ASC from a set of edges. The facets are determined from the specified edges.
         */
        template<typename E, typename... Edges>
        static AbstractSimplicialComplex create(E e, Edges... es) {
            return AbstractSimplicialComplex{}.addEdges(e, es...);
        }

        /**
         * This returns the vertex set, which is calculated by iterating over the facets.
         * @return the set of vertices
         */
        const vertices_t vertex_set() const noexcept;

        /**
         * This returns the entire edge set, which is calculated by determining all subsets of the facets;
         * thus, it is an intensive operation (up to O(2^v) where v is the number of vertices) and should be
         * used cautiously.
         * @return the set of all edges
         */
        const edges_t edge_set() const noexcept;

        /**
         * This returns the facets that fully define the ASC.
         * @return the set of facets
         */
        const edges_t &facet_set() const noexcept;
    };
}
