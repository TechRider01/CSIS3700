
#ifndef DISJOINTSET_H
#define DISJOINTSET_H
class DisjointSet {
	public:
		DisjointSet(uint32_t n) {
			elements = new uint32_t[n];
			rank = new uint32_t[n];
			for(int i =0; i<n; i++) {
				elements[i] = i;
				rank[i] = 0;
			}
		}
		
		~DisjointSet() {
			delete[] elements;
			delete[] rank;
		}
		
		uint32_t find(uint32_t a) {
			if(elements[a] != a) {
				elements[a] = find(elements[a]);
			}
			return elements[a];
		}
		
		void join(uint32_t a, uint32_t b) {
			a = find(a);
			b = find(b);
			if(a != b) {
				if(rank[a] < rank[b]) {
					elements[a] = b;
				} else {
					if (rank[a] == rank[b]) {
						rank[a]+=1;
					}
				elements[b] = a;
				}
			}
		}
		
	private:
		uint32_t
			*elements,
			*rank;
};

#endif
