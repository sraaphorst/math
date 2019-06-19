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
     */
    class AbstractSimplicialComplex final {
    public:
        using vertex_t   = int;
        using vertices_t = std::set<int>;
        using edge_t     = std::set<int>;
        using edges_t    = std::set<edge_t>;

    private:
        const edges_t facets{};

        // We have to manage facets explicitly to avoid introducing subset relationships, so force them to be
        // added one-at-a-time.
        explicit AbstractSimplicialComplex(edges_t) noexcept;

    public:
        AbstractSimplicialComplex() noexcept = default;
        AbstractSimplicialComplex(const AbstractSimplicialComplex&) = default;
        AbstractSimplicialComplex(AbstractSimplicialComplex&&) = default;

        ~AbstractSimplicialComplex() noexcept = default;

        bool operator==(const AbstractSimplicialComplex&) const noexcept;
        bool operator!=(const AbstractSimplicialComplex&) const noexcept;

        AbstractSimplicialComplex operator+(const edge_t&) const noexcept;

        template<typename E>
        AbstractSimplicialComplex addEdges(E e) {
            return *this + e;
        }

        template<typename E, typename... Edges>
        AbstractSimplicialComplex addEdges(E e, Edges... es) {
            return (*this + e).addEdges(es...);
        }

        template<typename E>
        static AbstractSimplicialComplex create(E e) {
            return AbstractSimplicialComplex{} + e;
        }

        template<typename E, typename... Edges>
        static AbstractSimplicialComplex create(E e, Edges... es) {
            return AbstractSimplicialComplex{}.addEdges(e, es...);
        }

        const vertices_t vertex_set() const noexcept;
        const edges_t edge_set() const noexcept;
        const edges_t &facet_set() const noexcept;
    };
}




