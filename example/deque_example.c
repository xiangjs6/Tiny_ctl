#include "../include/tctl_deque.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_int.h"
#include "../include/tctl_arg.h"
#include <stdio.h>
#define Import DEQUE, ITERATOR, INT
 
int test_deque_2()
{
    ARP_CreatePool();
    { // deque::at: Returns a reference to the element at a specified location in the deque.
        Deque c1 = new(T(Deque), T(Int), VAEND);
     
        THIS(c1).push_back(VA(10));
        THIS(c1).push_back(VA(20));
     
        const Int i = (void*)THIS(c1).brackets(VA(0));
        Int j = THIS(c1).brackets(VA(1));
        printf("The first element is %lld\n", i->val);
        printf("The second element is %lld\n", j->val);
        delete(c1);
    }
     
    { // deque::back: Returns a reference to the last element of the deque.
        Deque c1 = new(T(Deque), T(Int), VAEND);
     
        THIS(c1).push_back(VA(10));
        THIS(c1).push_back(VA(11));
     
        Int i = THIS(c1).back();
        Int ii = THIS(c1).front();
     
        printf("The last integer of c1 is %lld\n", i->val); // 11
        i->val--;
        printf("The next-to-last integer of c1 is %lld\n", ii->val); // 10
        printf("The last integer of c1 is %lld\n", ((Int)THIS(c1).back())->val); // 10
        delete(c1);
    }
     
    { // deque::clear: Erases all the elements of a deque.
        Deque c1 = new(T(Deque), T(Int), VAEND);

        THIS(c1).push_back(VA(10));
        THIS(c1).push_back(VA(20));
        THIS(c1).push_back(VA(30));
     
        printf("The size of the deque is initially %u\n", THIS(c1).size());
        THIS(c1).clear();
        printf("The size of the deque after clearing is %u\n", THIS(c1).size());
        delete(c1);
    }
     
    //这个还未完成
    /*
    { // deque::const_reference: A type that provides a reference to a const element stored in a deque for reading and performing const operations
        Deque c1 = new(T(Deque), VA(T(int)));
     
        THIS(c1).push_back(VA(10));
        THIS(c1).push_back(VA(20));
     
        const deque <int> c2 = c1;
        const int &i = c2.front();
        const int &j = c2.back();
        cout << "The first element is " << i << endl;
        cout << "The second element is " << j << endl;
     
        // The following line would cause an error as c2 is const
        // c2.push_back( 30 );
    }
    */
     
    //反向迭代器未写
    { // deque::crbegin: Returns a const iterator to the first element in a reversed deque
        Deque v1 = new(T(Deque), T(Int), VAEND);
        Iterator v1_Iter;
        //deque <int>::const_reverse_iterator v1_rIter;
     
        THIS(v1).push_back(VA(1));
        THIS(v1).push_back(VA(2));
     
        v1_Iter = THIS(v1).begin();
        printf("The first element of deque is %lld.\n", ((Int)THIS(v1_Iter).derefer())->val);
     
        //v1_rIter = v1.crbegin();
        //cout << "The first element of the reversed deque is " << *v1_rIter << "." << endl;
        delete(v1);
    }
     
    { // deque::emplace: Inserts an element constructed in place into the deque at a specified position.
        Deque v1 = new(T(Deque), T(Int), VAEND);
        Iterator Iter;
     
        THIS(v1).push_back(VA(10));
        THIS(v1).push_back(VA(20));
        THIS(v1).push_back(VA(30));
     
        printf("v1 =");
        for (Iter = THIS(v1).begin(); !THIS(Iter).equal(VA(THIS(v1).end())); THIS(Iter).inc())
            printf(" %lld", ((Int)THIS(Iter).derefer())->val);
        putchar('\n');
     
        // initialize a deque of deques by moving v1  
        /*
        deque < deque <int> > vv1;
     
        vv1.emplace(vv1.begin(), move(v1));
        if (vv1.size() != 0 && vv1[0].size() != 0) {
            cout << "vv1[0] =";
            for (Iter = vv1[0].begin(); Iter != vv1[0].end(); Iter++)
                cout << " " << *Iter;
            cout << endl;
        }
        */
        delete(v1);
    }
    ARP_FreePool();
    return 0;
}
     

// reference: http://www.cplusplus.com/reference/deque/deque/

int test_deque_1()
{
    ARP_CreatePool();
    { // deque::deque: Construct deque container
        //unsigned int i;

        // constructors used in the same order as described above:
        //Deque first = new(T(Deque), VA(T(int)));                                // empty deque of ints
        Deque second = new(T(Deque), T(Int), VA(4, 100), VAEND);                       // four ints with value 100
        Deque third = new(T(Deque), T(Int), THIS(second).begin(), THIS(second).end(), VAEND);  // iterating through second
        Deque fourth = new(T(Deque), T(Int), third, VAEND);                       // a copy of third

        // the iterator constructor can be used to copy arrays:
        //int myints[] = { 16, 2, 77, 29 };
        //std::deque<int> fifth(myints, myints + sizeof(myints) / sizeof(int));

        printf("The contents of fifth are:");
        for (Iterator it = THIS(fourth).begin(); !THIS(it).equal(VA(THIS(fourth).end())); THIS(it).inc())
            printf(" %lld", ((Int)THIS(it).derefer())->val); // 16 2 77 29
        putchar('\n');
    }

    //assign未实现
/*
    { // deque::assign: Assigns new contents to the deque container,
      // replacing its current contents, and modifying its size accordingly.
     
        std::deque<int> first;
        std::deque<int> second;
        std::deque<int> third;
     
        first.assign(7, 100);             // 7 ints with a value of 100
     
        std::deque<int>::iterator it;
        it = first.begin() + 1;
     
        second.assign(it, first.end() - 1); // the 5 central values of first
     
        int myints[] = { 1776, 7, 4 };
        third.assign(myints, myints + 3);   // assigning from array.
     
        std::cout << "Size of first: " << int(first.size()) << '\n'; // 7
        std::cout << "Size of second: " << int(second.size()) << '\n'; // 5
        std::cout << "Size of third: " << int(third.size()) << '\n'; // 3
    }
    */

     
    { // deque::at: Returns a reference to the element at position n in the deque container object.
        Deque mydeque = new(T(Deque), T(Int), VA(10), VAEND);   // 10 zero-initialized unsigneds
     
        // assign some values:
        for (unsigned i = 0; i < THIS(mydeque).size(); i++)
            ((Int)THIS(mydeque).brackets(VA(i)))->val = i;
     
        printf("mydeque contains:");
        for (unsigned i = 0; i < THIS(mydeque).size(); i++)
            printf(" %lld", ((Int)THIS(mydeque).brackets(VA(i)))->val); // 0 1 2 3 4 5 6 7 8 9
        putchar('\n');
        delete(mydeque);
    }
    
    { // deque::back: Returns a reference to the last element in the container.
      // deque::push_back: Adds a new element at the end of the deque container,
      // after its current last element. The content of val is copied (or moved) to the new element
        Deque mydeque = new(T(Deque), T(Int), VA(10), VAEND);   // 10 zero-initialized unsigneds
        THIS(mydeque).push_back(VA(10));

        while (((Int)THIS(mydeque).back())->val != 0)
            THIS(mydeque).push_back(VA(((Int)THIS(mydeque).back())->val - 1));
     
        printf("mydeque contains:");
        for (Iterator it = THIS(mydeque).begin(); !THIS(it).equal(VA(THIS(mydeque).end())); THIS(it).inc())
            printf(" %lld", ((Int)THIS(it).derefer())->val);
        putchar('\n');
        delete(mydeque);
    }
    
    { // deque::begin: Return iterator to beginning
      // deque::end: Return iterator to end
        Deque mydeque = new(T(Deque), T(Int), VAEND);
        for (int i = 1; i <= 5; i++) THIS(mydeque).push_back(VA(i));
     
        printf("mydeque contains:");
     
        for (Iterator it = THIS(mydeque).begin(); !THIS(it).equal(VA(THIS(mydeque).end())); THIS(it).inc())
            printf(" %lld", ((Int)THIS(it).derefer())->val);
        putchar('\n');
        delete(mydeque);
    }
    
/*
    { // deque::cbegin: c++11, Return const_iterator to beginning
      // deque::cend: c++11, Return const_iterator to end
        std::deque<int> mydeque = { 10, 20, 30, 40, 50 };
     
        std::cout << "mydeque contains:";
        for (auto it = mydeque.cbegin(); it != mydeque.cend(); ++it)
            std::cout << ' ' << *it;
        std::cout << '\n';
    }
    */

    
    { // deque::clear: Clear content
        //unsigned int i;
        Deque mydeque = new(T(Deque), T(Int), VAEND);
        THIS(mydeque).push_back(VA(100));
        THIS(mydeque).push_back(VA(200));
        THIS(mydeque).push_back(VA(300));
     
        printf("mydeque contains:");
        for (Iterator it = THIS(mydeque).begin(); !THIS(it).equal(VA(THIS(mydeque).end())); THIS(it).inc())
            printf(" %lld", ((Int)THIS(it).derefer())->val);
        putchar('\n');
     
        THIS(mydeque).clear();
        THIS(mydeque).push_back(VA(1101));
        THIS(mydeque).push_back(VA(2202));
     
        printf("mydeque contains:");
        for (Iterator it = THIS(mydeque).begin(); !THIS(it).equal(VA(THIS(mydeque).end())); THIS(it).inc())
            printf(" %lld", ((Int)THIS(it).derefer())->val);
        putchar('\n');
        delete(mydeque);
    }
    
/*
    { // deque::crbegin: c++11, Return const_reverse_iterator to reverse beginning
      // deque::crend: c++11, Return const_reverse_iterator to reverse end
        std::deque<int> mydeque = { 1, 2, 3, 4, 5 };
     
        std::cout << "mydeque backwards:";
        for (auto rit = mydeque.crbegin(); rit != mydeque.crend(); ++rit)
            std::cout << ' ' << *rit;
        std::cout << '\n';
    }
    *//*

    //emplace都未实现
    */
/*
    { // deque::emplace: c++11, Construct and insert element
        std::deque<int> mydeque = { 10, 20, 30 };
     
        auto it = mydeque.emplace(mydeque.begin() + 1, 100);
        mydeque.emplace(it, 200);
        mydeque.emplace(mydeque.end(), 300);
     
        std::cout << "mydeque contains:";
        for (auto& x : mydeque)
            std::cout << ' ' << x;
        std::cout << '\n';
    }
     
    { // deque::emplace_back: c++11, Construct and insert element at the end
        std::deque<int> mydeque = { 10, 20, 30 };
     
        mydeque.emplace_back(100);
        mydeque.emplace_back(200);
     
        std::cout << "mydeque contains:";
        for (auto& x : mydeque)
            std::cout << ' ' << x;
        std::cout << '\n';
    }
     
    { // deque::emplace_front: c++11, Construct and insert element at beginning
        std::deque<int> mydeque = { 10, 20, 30 };
     
        mydeque.emplace_front(111);
        mydeque.emplace_front(222);
     
        std::cout << "mydeque contains:";
        for (auto& x : mydeque)
            std::cout << ' ' << x;
        std::cout << '\n';
    }
    */

     
    { // deque::empty: Test whether container is empty
        Deque mydeque = new(T(Deque), T(Int), VAEND);
        long long sum = 0;
     
        for (int i = 1; i <= 10; i++) THIS(mydeque).push_back(VA(i));
     
        while (!THIS(mydeque).empty()) {
            sum += ((Int)THIS(mydeque).front())->val;
            THIS(mydeque).pop_front();
        }
     
        printf("total: %lld\n", sum);
    }
    
    { // deque::erase: Erase elements
        Deque mydeque = new(T(Deque), T(Int), VAEND);
     
        // set some values (from 1 to 10)
        for (int i = 1; i <= 10; i++) THIS(mydeque).push_back(VA(i));
     
        // erase the 6th element
        Iterator sixth = THIS(mydeque).begin();
        THIS(sixth).self_add(VA(5));
        THIS(mydeque).erase(sixth);
        //删除第七个
        THIS(mydeque).erase(THIS(THIS(mydeque).begin()).add(VA(5)));
     
        // erase the first 3 elements:
        // 批量没实现
        //mydeque.erase(mydeque.begin(), mydeque.begin() + 3);
     
        printf("mydeque contains:");
        for (Iterator it = THIS(mydeque).begin(); !THIS(it).equal(VA(THIS(mydeque).end())); THIS(it).inc())
            printf(" %lld", ((Int)THIS(it).derefer())->val);
        putchar('\n');
        delete(mydeque);
    }
    
    { // deque::front: Access first element, Returns a reference to the first element in the deque containe
      // deque::push_front: Inserts a new element at the beginning of the deque container,
      // right before its current first element. The content of val is copied (or moved) to the inserted element
        Deque mydeque = new(T(Deque), T(Int), VAEND);
     
        THIS(mydeque).push_front(VA(77));
        THIS(mydeque).push_front(VA(20));
     
        ((Int)THIS(mydeque).front())->val -= ((Int)THIS(mydeque).back())->val;
     
        printf("mydeque.front() is now %lld\n", ((Int)THIS(mydeque).front())->val);
    }
/*
    { // deque::get_allocator: Returns a copy of the allocator object associated with the deque object
        std::deque<int> mydeque;
        int * p;
        unsigned int i;
     
        // allocate an array with space for 5 elements using deque's allocator:
        p = mydeque.get_allocator().allocate(5);
     
        // construct values in-place on the array:
        for (i = 0; i < 5; i++) mydeque.get_allocator().construct(&p[i], i);
     
        std::cout << "The allocated array contains:";
        for (i = 0; i < 5; i++) std::cout << ' ' << p[i];
        std::cout << '\n';
     
        // destroy and deallocate:
        for (i = 0; i < 5; i++) mydeque.get_allocator().destroy(&p[i]);
        mydeque.get_allocator().deallocate(p, 5);
    }
    */


    { // deque::insert: Insert elements
        Deque mydeque = new(T(Deque), T(Int), VAEND);
     
        // set some initial values:
        for (int i = 1; i < 6; i++) THIS(mydeque).push_back(VA(i)); // 1 2 3 4 5
     
        Iterator it = THIS(mydeque).begin();
        THIS(it).inc();
     
        it = THIS(mydeque).insert(it, VA(10));                  // 1 10 2 3 4 5
        // "it" now points to the newly inserted 10
     
        //未实现
        //mydeque.insert(it, 2, 20);                     // 1 20 20 10 2 3 4 5
        // "it" no longer valid!
     
        //it = THIS(mydeque).begin();
        //THIS(it).self_add(2);
     
        //std::vector<int> myvector(2, 30);
        //mydeque.insert(it, myvector.begin(), myvector.end());
        // 1 20 30 30 20 10 2 3 4 5
     
        printf("mydeque contains:");
        for (Iterator it = THIS(mydeque).begin(); !THIS(it).equal(THIS(mydeque).end()); THIS(it).inc())
            printf(" %lld", ((Int)THIS(it).derefer())->val);
        putchar('\n');
        delete(mydeque);
    }
    
    { // deque::max_size: Return maximum size
        unsigned int i;
        Deque mydeque = new(T(Deque), T(Int), VAEND);
     
        i = 100;
     
        THIS(mydeque).resize(i);
        //if (i < mydeque.max_size()) mydeque.resize(i);
        //else std::cout << "That size exceeds the limit.\n";
        //fprintf(stderr, "max size: %d\n", mydeque.max_size());
        printf("size:%u\n", THIS(mydeque).size());
        delete(mydeque);
    }
/*
    { // deque::operator=: Assigns new contents to the container, replacing its current contents, and modifying its size accordingly
        std::deque<int> first(3);    // deque with 3 zero-initialized ints
        std::deque<int> second(5);   // deque with 5 zero-initialized ints
     
        second = first;
        first = std::deque<int>();
     
        std::cout << "Size of first: " << int(first.size()) << '\n';
        std::cout << "Size of second: " << int(second.size()) << '\n';
    }
    */


    { // deque::operator[]: Returns a reference to the element at position n in the deque container
        Deque mydeque = new(T(Deque), T(Int), VA(10), VAEND);
        size_t sz = THIS(mydeque).size();
     
        // assign some values:
        for (unsigned i = 0; i < sz; i++) ((Int)THIS(mydeque).brackets(VA(i)))->val = i;
     
        // reverse order of elements using operator[]:
        for (unsigned i = 0; i < sz / 2; i++) {
            long long temp;
            temp = ((Int)THIS(mydeque).brackets(VA(sz - 1 - i)))->val;
            ((Int)THIS(mydeque).brackets(VA(sz - 1 - i)))->val = ((Int)THIS(mydeque).brackets(VA(i)))->val;
            ((Int)THIS(mydeque).brackets(VA(i)))->val = temp;
        }
     
        // print content:
        printf("mydeque contains:");
        for (unsigned i = 0; i < sz; i++)
            printf(" %lld", ((Int)THIS(mydeque).brackets(VA(i)))->val);
        putchar('\n');
        delete(mydeque);
    }
    
    { // deque::pop_back: Removes the last element in the deque container, effectively reducing the container size by one
        Deque mydeque = new(T(Deque), T(Int), VAEND);
        long long sum = 0;
        THIS(mydeque).push_back(VA(10));
        THIS(mydeque).push_back(VA(20));
        THIS(mydeque).push_back(VA(30));
     
        while (!THIS(mydeque).empty()) {
            sum += ((Int)THIS(mydeque).back())->val;
            THIS(mydeque).pop_back();
        }
     
        printf("The elements of mydeque add up to %lld\n", sum);
        delete(mydeque);
    }
    
    { // deque::pop_front: Removes the first element in the deque container, effectively reducing its size by one.
        Deque mydeque = new(T(Deque), T(Int), VAEND);
     
        THIS(mydeque).push_back(VA(100));
        THIS(mydeque).push_back(VA(200));
        THIS(mydeque).push_back(VA(300));
     
        printf("Popping out the elements in mydeque:");
        while (!THIS(mydeque).empty()) {
            printf(" %lld", ((Int)THIS(mydeque).front())->val);
            THIS(mydeque).pop_front();
        }
        putchar('\n');
        printf("The final size of mydeque is %u\n", THIS(mydeque).size());
        delete(mydeque);
    }
/*
    { // deque::rbegin: Returns a reverse iterator pointing to the last element in the container
        // deque::rend: Returns a reverse iterator pointing to the theoretical element preceding the first element in the deque container
        std::deque<int> mydeque(5);  // 5 default-constructed ints
     
        std::deque<int>::reverse_iterator rit = mydeque.rbegin();
     
        int i = 0;
        for (rit = mydeque.rbegin(); rit != mydeque.rend(); ++rit)
            *rit = ++i;
     
        std::cout << "mydeque contains:";
        for (std::deque<int>::iterator it = mydeque.begin(); it != mydeque.end(); ++it)
            std::cout << ' ' << *it;
        std::cout << '\n';
    }
    */

    { // deque::resize: Resizes the container so that it contains n elements
        Deque mydeque = new(T(Deque), T(Int), VAEND);
     
        // set some initial content:
        for (int i = 1; i < 10; ++i) THIS(mydeque).push_back(VA(i));
     
        THIS(mydeque).resize(5);
        //THIS(mydeque).resize(8, 100);
        THIS(mydeque).resize(12);
     
        for (Iterator it = THIS(mydeque).begin(); !THIS(it).equal(VA(THIS(mydeque).end())); THIS(it).inc())
            printf(" %lld", ((Int)THIS(it).derefer())->val);
        putchar('\n');
        delete(mydeque);
    }
/*
    { // deque::shrink_to_fit: c++11, Requests the container to reduce its memory usage to fit its size.
        // deque::size: Returns the number of elements in the deque container
        std::deque<int> mydeque(100);
        std::cout << "1. size of mydeque: " << mydeque.size() << '\n';
     
        mydeque.resize(10);
        std::cout << "2. size of mydeque: " << mydeque.size() << '\n';
     
        mydeque.shrink_to_fit();
        fprintf(stderr, "3. size of mydeque: %d\n", mydeque.size());
    }
    */

    { // deque::swap: Exchanges the content of the container by the content of x,
        // which is another deque object containing elements of the same type. Sizes may differ.
        //unsigned int i;
        Deque foo = new(T(Deque), T(Int), VA(3, 100), VAEND);// three ints with a value of 100
        Deque bar = new(T(Deque), T(Int), VA(5, 200), VAEND);// five ints with a value of 200
        
     
        THIS(foo).swap(bar);
     
        printf("foo contains:");
        for (Iterator it = THIS(foo).begin(); !THIS(it).equal(VA(THIS(foo).end())); THIS(it).inc())
            printf(" %lld", ((Int)THIS(it).derefer())->val);
        putchar('\n');
     
        printf("bar contains:");
        for (Iterator it = THIS(bar).begin(); !THIS(it).equal(VA(THIS(bar).end())); THIS(it).inc())
            printf(" %lld", ((Int)THIS(it).derefer())->val);
        putchar('\n');
    }
/*
    { // relational operators: compare
        std::deque<int> foo(3, 100);   // three ints with a value of 100
        std::deque<int> bar(2, 200);   // two ints with a value of 200
     
        if (foo == bar) std::cout << "foo and bar are equal\n";
        if (foo != bar) std::cout << "foo and bar are not equal\n";
        if (foo< bar) std::cout << "foo is less than bar\n";
        if (foo> bar) std::cout << "foo is greater than bar\n";
        if (foo <= bar) std::cout << "foo is less than or equal to bar\n";
        if (foo >= bar) std::cout << "foo is greater than or equal to bar\n";
    }
     
    { // std::swap: The contents of container x are exchanged with those of y.
      // Both container objects must be of the same type (same template parameters), although sizes may differ
        unsigned int i;
        std::deque<int> foo(3, 100);   // three ints with a value of 100
        std::deque<int> bar(5, 200);   // five ints with a value of 200
     
        swap(foo, bar);
     
        std::cout << "foo contains:";
        for (std::deque<int>::iterator it = foo.begin(); it != foo.end(); ++it)
            std::cout << ' ' << *it;
        std::cout << '\n';
     
        std::cout << "bar contains:";
        for (std::deque<int>::iterator it = bar.begin(); it != bar.end(); ++it)
            std::cout << ' ' << *it;
        std::cout << '\n';
    }
    */

    ARP_FreePool();
	return 0;
}



int main(void)
{
    printf("stage1\n");
    test_deque_1();
    printf("stage2\n");
    test_deque_2();
    return 0;
}
