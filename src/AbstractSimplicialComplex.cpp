/**
 * AbstractSimplicialComplex.cpp
 *
 * By Sebastian Raaphorst, 2019.
 */

#include <algorithm>
#include <iterator>
#include <set>
#include <vector>

#include "AbstractSimplicialComplex.h"

namespace vorpal::algebra {
    static bool is_subset(const AbstractSimplicialComplex::edge_t &sub, const AbstractSimplicialComplex::edge_t &sup) {
        // Immediately reject if the candidate subset is bigger.
        if (sub.size() > sup.size())
            return false;

        return std::all_of(std::cbegin(sub), std::cend(sub), [&sup](const auto &elem) {
            return sup.find(elem) != std::cend(sup);
        });
    }

    static AbstractSimplicialComplex::edges_t allNonEmptySubEdges(const AbstractSimplicialComplex::edge_t &edge) noexcept {
        if (edge.empty())
            return AbstractSimplicialComplex::edges_t{};

        // Convert to a vector because we need random access.
        std::vector<int> v{};
        v.insert(std::begin(v), std::cbegin(edge), std::cend(edge));

        std::vector<std::vector<int>> subsets;
        subsets.emplace_back(std::vector<int>{});

        for (const auto elem: edge) {
            std::vector<std::vector<int>> tmp = subsets;

            for (auto &subset: tmp)
                subset.emplace_back(elem);

            subsets.insert(std::end(subsets), std::cbegin(tmp), std::cend(tmp));
        }

        // Convert to sets.
        AbstractSimplicialComplex::edges_t edges{};
        for (const auto &subset: subsets) {
            if (subset.empty())
                continue;
            AbstractSimplicialComplex::edge_t e{std::cbegin(subset), std::cend(subset)};
            edges.insert(e);
        }

        return edges;
    }

    AbstractSimplicialComplex::AbstractSimplicialComplex(edges_t facets) noexcept:
        facets{std::move(facets)} {}

    bool AbstractSimplicialComplex::operator==(const AbstractSimplicialComplex &other) const noexcept {
        return facets == other.facets;
    }

    bool AbstractSimplicialComplex::operator!=(const AbstractSimplicialComplex &other) const noexcept {
        return *this != other;
    }

    AbstractSimplicialComplex AbstractSimplicialComplex::operator+(const edge_t &edge) const noexcept {
        // If this is already a facet, ignore.
        if (facets.find(edge) != std::cend(facets))
            return *this;

        // If this is a sub-edge of a facet, ignore.
        if (std::any_of(std::cbegin(facets), std::cend(facets), [&edge](const auto &facet) {
            return is_subset(edge, facet);
        }))
            return *this;

        edges_t new_facets;

        // If any of the existing facets are subedges of this facet, do not add them.
        std::for_each(std::cbegin(facets), std::cend(facets), [&new_facets, &edge] (const auto &prev_facet){
            if (!is_subset(prev_facet, edge))
                new_facets.insert(prev_facet);
        });

        // Add the new facet.
        new_facets.insert(edge);
        return AbstractSimplicialComplex(new_facets);
    }

    const AbstractSimplicialComplex::vertices_t AbstractSimplicialComplex::vertex_set() const noexcept {
        vertices_t vertices;
        std::for_each(std::cbegin(facets), std::cend(facets), [&vertices](const auto &facet) {
            vertices.insert(std::cbegin(facet), std::cend(facet));
        });
        return vertices;
    }

    const AbstractSimplicialComplex::edges_t AbstractSimplicialComplex::edge_set() const noexcept {
        edges_t edges;

        for (const auto &facet: facets) {
            const auto subfacets = allNonEmptySubEdges(facet);
            edges.insert(std::cbegin(subfacets), std::cend(subfacets));
        }

        return edges;
    }

    const AbstractSimplicialComplex::edges_t &AbstractSimplicialComplex::facet_set() const noexcept {
        return facets;
    }
}