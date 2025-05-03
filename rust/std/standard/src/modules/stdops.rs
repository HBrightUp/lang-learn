
use std::{
    ops::{
        Bound::*,RangeBounds,ControlFlow,
    },
    collections::{
        BTreeMap,
    },
};

use crate::include::{
    point::{
        Point,
    },
};

pub fn stdops() {

    // Call  Point::PartialEq trait
    assert_eq!(Point{x: 3, y: 3}, Point{x: 0, y: 1} + Point{x: 3, y: 2});
    assert_eq!(Point {x: -1, y: -3}, Point {x: 1, y: 0} - Point {x: 2, y: 3});

    //Fn
    {
        let double = |x| x * 2;
        assert_eq!(call_with_once(double), 6);
    }
    //FnMut
    {
        let mut x: usize = 1;
        let add_two_to_x = || x += 2;
        do_twice(add_two_to_x);
        assert_eq!(x, 5);
    }

    //FnOnce 
    {
        let x = String::from("x");
        let consume_and_return_x = move || x;
        consume_with_relish(consume_and_return_x);

        //Re-use prohibited
        // consume_and_return_x 
    }

    //Range:
    {
        assert_eq!((3..5), std::ops::Range{start: 3, end: 5});
        assert_eq!(3 + 4 + 5, (3..6).sum());

        let arr = [1, 2, 3, 4, 5];
        assert_eq!(arr[..=2], [1, 2, 3]);
        assert_eq!(arr[..], [1, 2, 3, 4, 5]);

        assert!(!(3..5).contains(&2));
        assert!( (0.0..1.0).contains(&0.5));
        assert!(!(0.0..f32::NAN).contains(&0.5));

        assert!(!(3..5).is_empty());
        assert!( (3..2).is_empty());
        assert!(!(3.0..5.0).is_empty());
    }

    //enum Bound
    {
        assert_eq!((..100).start_bound(), Unbounded);
        assert_eq!((1..12).start_bound(), Included(&1));
        assert_eq!((1..12).end_bound(), Excluded(&12));

        let mut map = BTreeMap::new();
        map.insert(3, 'a');
        map.insert(5, 'b');
        map.insert(8, 'c');

        //5:b   8:c
        // range: Search for data in the specified range;
        for(_key, _value) in map.range((Excluded(3), Included(8))) {
            //println!("{key}:{value}");
        }
        assert_eq!(Some((&3, &'a')), map.range((Unbounded, Included(5))).next());

        let bound_string = Included("Hello, world!");
        assert_eq!(bound_string.map(|s| s.len()), Included(13));

        //cloned:  Bound<&T> -> Bound<T>
        assert_eq!((1..12).start_bound(), Included(&1));
        assert_eq!((1..12).start_bound().cloned(), Included(1));
    }
    
    //std::ops::ControlFlow
    {   
        let r = (2..100).try_for_each(|x| {
            if 403 % x == 0 {
                return ControlFlow::Break(x)
            } 

            ControlFlow::Continue(())
        });

        assert_eq!(r, ControlFlow::Break(13));
    }
}

fn consume_with_relish<F>(func: F)
where 
    F: FnOnce() -> String 
{
    func();

    //Re-use prohibited
    // func() 
}

fn call_with_once<F>(func: F) -> usize 
where
    F: Fn(usize) -> usize 
{
    func(3)
}

fn do_twice<F>(mut func:  F) 
where
    F: FnMut() 
{
    func();
    func();
}



