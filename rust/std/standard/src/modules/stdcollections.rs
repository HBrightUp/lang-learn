

use std::{
    collections::{     
        VecDeque, LinkedList, BinaryHeap, BTreeSet, HashMap, 
        btree_set::
        {
            Entry::*,
        },
    },
    ops::{
        Bound,
    },
};



pub fn stdcollections() {
    
    // BinaryHeap 
    {
        let mut b1 = BinaryHeap::new();
        b1.push(3);
        b1.push(6);
        b1.push(100);
        b1.push(20);
    
        assert_eq!(b1.len(), 4);
        assert_eq!(b1.peek(), Some(&100));
        b1.clear();
        assert!(b1.is_empty());
    }


    //HashMap
    {
        let mut h1 = HashMap::with_capacity(1);

        // 'insert' function maybe overwrite
        h1.insert("Mercury", 10);
        h1.insert("Venus", 40);
        h1.insert("Earth", 20);
        h1.insert("Mars", 100);
        
        // The hashmap capacity automatically increases according to the rule of 2N+1, like thsi：1 -> 3 -> 7
        assert_eq!(h1.capacity(), 7);
        
        // Whether the key exists
        assert!(h1.contains_key("Earth"));

        // Modify value
        if let Some(mars) = h1.get_mut("Mars") {
            *mars -= 10;
            assert_eq!(h1.get("Mars"), Some(&90));
        }

        
        // Traverse and modify values
        for(_, val) in h1.iter_mut() {
            if *val < 30 {
                *val += 10;
            }
        }
        assert_eq!(h1.get("Mercury"), Some(&20));
        assert_eq!(h1.remove(&"Mercury"), Some(20));

        //clear
        h1.clear();

        // Example: Count the number of h2 in a str
        let mut h2 = HashMap::new();

        for ch in "a short treatise on fungi".chars() {
            h2.entry(ch).and_modify(|counter| *counter += 1).or_insert(1);
        }
        assert_eq!(h2[&'s'], 2);
        assert_eq!(h2[&'t'], 3);
        assert_eq!(h2[&'u'], 1);
    }

    //BTreeSet
    {
        let mut b1 = BTreeSet::new();
        b1.insert(10);
        b1.insert(20);
        b1.insert(15);
        b1.insert(8);
        assert!(b1.contains(&15));

        let mut b2 = BTreeSet::new();
        b2.insert(60);
        b2.insert(20);

        //Get the differences elements of two BTreeSets and put the results into a vector
        let d1: Vec<_> = b1.symmetric_difference(&b2).cloned().collect();
        assert_eq!(d1, [8, 10, 15, 60]);

        // Get the same elements of two BTreeSets and put the results into a vector
        let  d2: Vec<_> = b1.intersection(&b2).cloned().collect();
        assert_eq!(d2, [20]);  

        // Merge two BTreeSet
        let union_b1: Vec<_> = b1.union(&b2).cloned().collect();
        assert_eq!(union_b1, [8, 10, 15, 20, 60]);

        // Determine whether b1 is a subset of b2
        assert_eq!(b1.is_subset(&b2), false);

        // Determine whether b2 is a subset of b1
        assert_eq!(b1.is_superset(&b2), false);

        assert_eq!(b1.first(), Some(&8));
        assert_eq!(b1.last(), Some(&20));
        assert_eq!(b1.pop_first(), Some(8));
        assert!(!b1.contains(&8));
        assert_eq!(b1.get_or_insert(8), &8);
        assert_eq!(b1.remove(&8), true);
        assert_eq!(b1.get_or_insert(8), &8);
        assert_eq!(b1.take(&8), Some(8));
    }
    

    //BTreeSet Example: In "a short treatise on fungi", single occurrences of letters are stored in singles,
    // and multiple occurrences of letters are stored in multiple;
    {
        let mut singles = BTreeSet::new();
        let mut multiple = BTreeSet::new();

        for ch in "a short treatise on fungi".chars() {

            //如果 char 在 multiple中不存在
            if let Vacant(dupe_entry) = multiple.entry(ch) {
                match singles.entry(ch) {
                    Vacant(single_entry) => {
                        single_entry.insert()
                    }
                    Occupied(single_entry) => {
                        single_entry.remove();
                        dupe_entry.insert()
                    }
                }
            }
        }

        //singles: {'f', 'g', 'h', 'u'}
        //multiple: {'f', 'g', 'h', 'u'}
        assert!(!singles.contains(&'t') && multiple.contains(&'t'));
        assert!(singles.contains(&'u') && !multiple.contains(&'u'));
        assert!(!singles.contains(&'v') && !multiple.contains(&'v'));        
        assert_eq!(singles.difference(&BTreeSet::from(['f', 'g', 'h', 'u'])).cloned().collect::<Vec<_>>(), vec![]);

        // retain: Remove all elements where the condition is false;
        let mut s1 = BTreeSet::from([1, 2, 3, 4, 5, 6]);
        s1.retain(|&k| k % 2 == 0);
        assert!(s1.iter().eq([2, 4, 6].iter()));

        //append: Put the elements in s2 into s1 and clear s2;
        let mut s2 = BTreeSet::from([6, 7]);
        s1.append(&mut s2);
        assert_eq!(s1.len(), 4);
        assert_eq!(s2.len(), 0);
        //s1: {2, 4, 6, 7}

        //split_off
        let mut s3 = s1.split_off(&3);
        //  s1: {2};  s3: {4, 6, 7};

        //extract_if: 
        let s4: BTreeSet<_> = s3.extract_if(|v| v % 2 == 0).collect();
        //  s4: {4, 6}

        let mut s4_iter = s4.iter();
        assert_eq!(s4_iter.next(), Some(&4));

        //lower_bound/lower_bound_mut/upper_bound /upper_bound_mut;
        //Included,Excluded,Unbounded
        let set = BTreeSet::from([1, 2, 3, 4]);

        let cursor = set.lower_bound(Bound::Included(&2));
        assert_eq!(cursor.peek_prev(), Some(&1));
        assert_eq!(cursor.peek_next(), Some(&2));

        let cursor = set.lower_bound(Bound::Excluded(&2));
        assert_eq!(cursor.peek_prev(), Some(&2));
        assert_eq!(cursor.peek_next(), Some(&3));

        let cursor = set.lower_bound(Bound::Unbounded);
        assert_eq!(cursor.peek_prev(), None);
        assert_eq!(cursor.peek_next(), Some(&1));   
    }
    
    //LinkedList: Doubly Linked List
    {
        let mut n1 = LinkedList::new();
        n1.push_back(100);
        n1.push_back(20);
    
        assert!(!n1.is_empty());
        assert_eq!(n1.len(), 2);
        assert!(n1.contains(&20));
        assert_eq!(n1.back(), Some(&20));
    
        n1.push_front(10);
        n1.clear();
        assert_eq!(n1.pop_back(), None);
    }


    //VecDeque
    {
        let mut  q1: VecDeque<i32> = VecDeque::with_capacity(100);
        q1.push_back(5);
        q1.push_back(6);
        q1.push_back(7);
        assert_eq!(q1.get(1), Some(&6));

        //modify value
        if let Some(first) = q1.get_mut(1) {
            *first = 8;
        }
        assert_eq!(q1[1], 8);

        //swap value 
        q1.swap(0, 1);
        assert_eq!(q1[0], 8);

        //truncate: holds the first N elements
        q1.truncate(2);
        assert_eq!(q1.len(), 2);

        //modify value with iter_mut
        for data in q1.iter_mut() {
            *data *= 2;
        }

        //q1: [16, 10]
        q1.push_front(2);
        q1.push_front(8);

        //range_mut: Modify the specified range of elements
        for v in q1.range_mut(2..) {
            *v *= 2;
        }
        assert_eq!(q1, [8, 2, 32, 20]);

        //drain: remove the specified range of elements
        let drained = q1.drain(2..).collect::<VecDeque<_>>();
        assert_eq!(q1, [8, 2]);
        assert_eq!(drained, [32, 20]);

        q1.insert(1, 100);
        assert_eq!(q1[1], 100);

        assert_eq!(q1.remove(1), Some(100));
        assert_eq!(q1, [8, 2]);

        q1.push_front(3);
        q1.push_back(30);
        //q1:  [3, 8, 2, 30]
        
        //make_contiguous: Rearranges this deque's internal storage into a contiguous slice and returns it.
        //sorted with ASC
        q1.make_contiguous().sort();
        assert_eq!(q1.as_slices(), (&[2, 3, 8, 30] as &[_], &[] as &[_]));

        // rotate_left/rotate_right
        q1.rotate_left(2);
        assert_eq!(q1, [8, 30, 2, 3]);

        //binary_search / binary_search_by: search element and return the index if existed.
        //Remark：this queue must be sourted.
        assert_eq!(q1.binary_search(&2),  Ok(2));
        q1.make_contiguous().sort();
        assert_eq!(q1.binary_search_by(|x| x.cmp(&3)),  Ok(1));

        //q1: [2, 3, 8, 30]

        //partition_point: search the first element with true and return index;
        let i = q1.partition_point(|&x| x < 5);
        assert_eq!(i, 2);
        assert!(q1.iter().take(i).all(|&x| x < 5));
        assert!(q1.iter().skip(i).all(|&x| !(x < 5)));

        q1.clear();

        //binary_search_by_key: 
        let q2: VecDeque<_> = [(0, 0), (2, 1), (4, 1), (5, 1),
            (3, 1), (1, 2), (2, 3), (4, 5), (5, 8), (3, 13),
            (1, 21), (2, 34), (4, 55)].into();
        
        //search "b = 13" and return index;   
        assert_eq!(q2.binary_search_by_key(&13, |&(_a, _b)| _b),  Ok(9));

        assert_eq!(q2.binary_search_by_key(&4, |&(_a, _b)| _b),   Err(7));
        assert_eq!(q2.binary_search_by_key(&100, |&(_a, _b)| _b), Err(13));
        let r = q2.binary_search_by_key(&1, |&(_a, _b)| _b);
        assert!(matches!(r, Ok(1..=4)));
    }
}


