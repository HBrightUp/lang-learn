
#[allow(unused_variables)]

use std::{
    collections::{
        BTreeMap, VecDeque, LinkedList, HashSet, BinaryHeap, BTreeSet, HashMap,
    },
    sync::{
        Arc,
    },
    net::{
        Ipv4Addr, IpAddr, Ipv6Addr,
    },
    hash::{
        BuildHasher,
    },
    rc::{
        Rc,
    },
};


// std array
pub fn stdarray() {

    //init
    {
        let mut arr: [i32; 3] = [0; 3];
        arr[1] = 1;

        let mut index = 0;
        for x in arr {
            assert_eq!(x, arr[index]);
            index += 1;
        }
    }
        
    // String -> [u8];  u8 -> str
    {
        const HEX_DIGITS: [std::ascii::Char; 16] = *b"0123456789abcdef".as_ascii().unwrap();
        assert_eq!(HEX_DIGITS[1].as_str(), "1");
        assert_eq!(HEX_DIGITS[10].as_str(), "a");
    }

    // tuple -> [T]
    {
        let tuple :(i32, i32, i32) = (1,2,3);
        let _arr: [i32; 3] = tuple.into();
    }

    // traverse value
    {
        let arr: [i32; 3] = [1, 2, 3];

        for item in arr.iter().enumerate() {
            let (_i, _x): (usize, &i32) = item;
            //println!("arr[{i}] = {x}");
        }

        for item in arr.into_iter().enumerate() {
            let (_i, _x):(usize, i32) = item;
            //println!("arr[{i}] = {x}");
        }

        for item in IntoIterator::into_iter(arr).enumerate() {
            let (_i, _x): (usize, i32) = item;
            //println!("arr[{i}] = {x}");
        }
    }

    //map for enum array, only for little data
    {
        let mut temp = 0;
        let arr = [1, 2, 3];
        let arr2 = arr.map(|v| {
            temp += 1;
            v * temp
        });
        
        assert_eq!(arr, [1,2,3]);
        assert_eq!(arr2, [1,4,9]);
    }

    //split array
    {
        let  arr = [1,2,3,4,5,6];
        let (left, right) = arr.split_array_ref::<3>();
        assert_eq!(left, &[1,2,3]);
        assert_eq!(right, &[4,5,6]);
    }
  
    // [Some(T), usize] -> Some([T; usize])
    {
        let arr = [Some(1); 1000];
        let arr: Option<[u8; 1000]> = arr.transpose();
        assert_eq!(arr, Some([1; 1000]));
    }
  
    // [T] -> Vec<T>
    assert_eq!(Vec::from([1,2,3]), vec![1,2,3]);

    // [T] -> BTreeMap
    {
        let m1 = BTreeMap::from([(1,2), (3,4)]);
        let m2 : BTreeMap<_, _> = [(1,2), (3,4)].into();
        assert_eq!(m1, m2);
    }

    // [T] -> HashMap
    {
        let m1 = HashMap::from([(1, 2), (3, 4)]);
        let m2: HashMap<_, _> = [(1, 2), (3, 4)].into();
        assert_eq!(m1, m2);
    }
    
    // [T] -> Arc<[T]>
    {
        let arr: [i32; 3] = [1,2,3];
        let shared: Arc<[i32]> = Arc::from(arr);
        assert_eq!(&[1,2,3], &shared[..]);
    }
 
    // [T] -> BTreeSet
    {
        let s1 = BTreeSet::from([1,2,3,4]);
        let s2: BTreeSet<_> = [1,2,3,4].into();
        assert_eq!(s1, s2);
    }

   // [T] -> BinaryHeap
   {
        let mut h1 = BinaryHeap::from([1, 4, 2, 3]);
        let mut h2: BinaryHeap<_> = [1, 4, 2, 3].into();
        while let Some((a, b)) = h1.pop().zip(h2.pop()) {
            assert_eq!(a, b);
        }
   }

    // [T] -> Box<[T]> 
    let _boxed: Box<[u8]> = Box::from([4, 2]);
 
    // [T] -> HashSet
    {
        let s1 = HashSet::from([1, 2, 3, 4]);
        let s2: HashSet<_> = [1, 2, 3, 4].into();
        assert_eq!(s1, s2);
    }

    // [T] -> LinkedList
    {
        let l1 = LinkedList::from([1, 2, 3, 4]);
        let l2: LinkedList<_> = [1, 2, 3, 4].into();
        assert_eq!(l1, l2);
    }

    // [T] -> Rc<[T]>
    {
        let arr: [i32; 3] = [1, 2, 3];
        let shared: Rc<[i32]> = Rc::from(arr);
        assert_eq!(&[1, 2, 3], &shared[..]);
    }
    
    // [T] ->  VecDeque
    {
        let d1 = VecDeque::from([1, 2, 3, 4]);
        let d2: VecDeque<_> = [1, 2, 3, 4].into();
        assert_eq!(d1, d2);
    }

    // [u16] -> IpAddr
    {
        let ip = IpAddr::from([
            0x20du16, 0x20cu16, 0x20bu16, 0x20au16,
            0x209u16, 0x208u16, 0x207u16, 0x206u16,
        ]);
        assert_eq!(
            IpAddr::V6(Ipv6Addr::new(
                0x20d, 0x20c, 0x20b, 0x20a,
                0x209, 0x208, 0x207, 0x206,
            )),
            ip
        );
    }


    // [u8] -> Ipv4Addr
    {
        let ip = Ipv4Addr::from([13u8, 12u8, 11u8, 10u8]);
        assert_eq!(Ipv4Addr::new(13, 12, 11, 10), ip);  
    }
    

    // [T] ->  BuildHasher
    {
        let b = std::hash::RandomState::new();
        let a: [u8; 3] = [0xa8, 0x3c, 0x09];
        let s: &[u8] = &[0xa8, 0x3c, 0x09];
        assert_eq!(b.hash_one(a), b.hash_one(s)); 

        //hash: 5979966261031055430
        //println!("hash: {:?}", b.hash_one(a));
    }

}



