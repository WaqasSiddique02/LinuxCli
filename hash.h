#include <string>
using namespace std;

class Hash {
public:
    static size_t hashFunction(const string& key, size_t bucketCount) {
        size_t hash = 0;
        for (size_t i = 0; i < key.length(); ++i) {
            hash = (hash * 31 + key[i]) % bucketCount;
        }
        return hash;
    }
};