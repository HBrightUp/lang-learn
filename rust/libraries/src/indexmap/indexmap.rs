use indexmap::{
    indexmap, indexset,IndexMap,
};

pub fn enter() {


}


#[cfg(test)]
mod test {

    use super::*;

    #[test] 
    fn test_map_sort() {
        let m = indexmap! {
            1 => 2,
            7 => 1,
            2 => 2,
            3 => 3,
        };

        itertools::assert_equal(
            m.sorted_by(| _k1, v1, _k2, v2 | v1.cmp(v2) ),
            vec!( (7,1), (1,2), (2,2), (3,3)),
        );
    }

    #[test]
    fn test_set_sort() {
        let s = indexset!{1, 7, 2, 3};
        itertools::assert_equal(s.sorted_by( |v1, v2| v1.cmp(v2)), vec![1, 2, 3, 7]);
    }

    #[test]
    fn test_base() {
        let mut letters = IndexMap::new();
        for ch in "a short treatise on fungi".chars() {
            *letters.entry(ch).or_insert(0) += 1;
        }

        assert_eq!(letters[&'s'], 2);
        assert_eq!(letters[&'t'], 3);
        assert_eq!(letters[&'u'], 1);
        assert_eq!(letters.get(&'y'), None);
    }

    #[test] 
    fn test_keys() {

        let mut map = IndexMap::new();
        for word in "Lorem ipsum dolor sit amet".split_whitespace() {
            map.insert(word.to_lowercase(), word.to_uppercase());
        }

        assert_eq!(map[0], "LOREM");
        assert_eq!(map.keys()[0], "lorem");
        assert_eq!(map[1], "IPSUM");
        assert_eq!(map.keys()[1], "ipsum");

        map.reverse();
        assert_eq!(map.keys()[0], "amet");
        assert_eq!(map.keys()[1], "sit");

        map.sort_keys();
        assert_eq!(map.keys()[0], "amet");
        assert_eq!(map.keys()[1], "dolor");

        // Advancing the iterator will offset the indexing
        let mut keys = map.keys();
        assert_eq!(keys[0], "amet");

        //INFO: 'Next' iter make the key index moved.
        assert_eq!(keys.next().map(|s| &**s), Some("amet"));
        assert_eq!(keys[0], "dolor");
        assert_eq!(keys[1], "ipsum");

        // Slices may have an offset as well
        let slice = &map[2..];
        assert_eq!(slice[0], "IPSUM");
        assert_eq!(slice.keys()[0], "ipsum");
    }

}