#include <iostream>
#include <AbstractSimplicialComplex.h>

using namespace vorpal::algebra;

using s=std::set<int>;

std::ostream &operator<<(std::ostream &out, const AbstractSimplicialComplex::edge_t &edge) {
    int curr_size = 0;
    int size = edge.size();

    out << '{';
    for (const auto &elem: edge) {
        if (curr_size && curr_size < size)
            out << ',';
        out <<elem;
        ++curr_size;
    }
    out << '}';
    return out;
}

int main() {
    // We can create an empty ASC.
    AbstractSimplicialComplex asc{};

    // These are equivalent.
    const auto a2 = AbstractSimplicialComplex::create(s{1,2,3}, s{4,5,6}, s{1,3,5,7}, s{1,5,7});
    const auto a3 = AbstractSimplicialComplex::create(s{4,5,6}, s{1,5,7}, s{1,2,3}, s{1,3,5,7}, s{3,5,7}, s{6,5,4});
    std::cout << std::boolalpha << "The ASCs are equal: " << (a2 == a3) << '\n';

    std::cout << "\n* FACETS *\n";
    for (const auto &e: a3.facet_set())
        std::cout << e << '\n';

    std::cout << "\n* FACETS *\n";
    for (const auto &e: a2.facet_set())
        std::cout << e << '\n';

    std::cout << "\n* VERTICES *\n";
    for (const auto i: a2.vertex_set())
        std::cout << i << ' ';
    std::cout << '\n';

    std::cout << "\n* EDGES *\n";
    for (const auto &e: a2.edge_set())
        std::cout << e << '\n';
}