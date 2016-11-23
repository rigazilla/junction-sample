// A basic sample application using Junction.

#include <junction/ConcurrentMap_Grampa.h>
#include <iostream>

class Foo {
  public:
	Foo(int a) : a(a) {}
  int a;
};

struct IntKeyTraits {
    typedef int Key;
    typedef typename turf::util::BestFit<int>::Unsigned Hash;
    static const Key NullKey = Key(0);
    static const Hash NullHash = Hash(0);
    static Hash hash(int key) {
         if (key % 2)
          return 42;
         else return 43;
    }
};

void testMap() {
    // Create the map.
    typedef junction::ConcurrentMap_Grampa<turf::u32, Foo*, IntKeyTraits> ConcurrentMap;
    ConcurrentMap myMap;

    // Do a few operations.
    myMap.assign(14, new Foo(14));
    myMap.assign(15, new Foo(15));
    myMap.assign(16, new Foo(16));
    myMap.assign(17, new Foo(17));
    Foo* foo14 = myMap.get(14);
    Foo* foo15 = myMap.get(15);
    Foo* foo16 = myMap.get(16);
    Foo* foo17 = myMap.get(17);
    std::cout << "Value for key 14 is " << foo14->a << std::endl;
    std::cout << "Value for key 15 is " << foo15->a << std::endl;
    std::cout << "Value for key 16 is " << foo16->a << std::endl;
    std::cout << "Value for key 17 is " << foo17->a << std::endl;
}

int main() {
    // Create QSBR context for the main thread.
    junction::QSBR::Context context = junction::DefaultQSBR.createContext();

    // Run a simple map test.
    testMap();

    // Update the QSBR context for this thread.
    // In a larger application, this should be called periodically, for each thread, at a moment
    // when the thread is quiescent – that is, not in the middle of any operation that uses a
    // Junction data structure.
    junction::DefaultQSBR.update(context);

    // Destroy the QSBR context for the main thread.
    junction::DefaultQSBR.destroyContext(context);
    return 0;
}
