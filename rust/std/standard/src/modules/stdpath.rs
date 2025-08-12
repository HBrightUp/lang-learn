use std::{
    path::{
        Path,PathBuf,
    },
    ffi::{
        OsStr, 
    },
};

pub fn stdpath() {

    //path
    {
        let path = Path::new("./some/dir/world.txt");

        let parent = path.parent();
        assert_eq!(parent, Some(Path::new("./some/dir")));

        let file_stem = path.file_stem();
        assert_eq!(file_stem, Some(OsStr::new("world")));

        let extension = path.extension();
        assert_eq!(extension, Some(OsStr::new("txt")));

        assert_eq!(path.to_str(), Some("./some/dir/world.txt"));
        assert_eq!(path.to_string_lossy(), "./some/dir/world.txt");
    }

    //Path -> str and [str] -> PathBuf
    {
        let mut p = PathBuf::from("/home/hml");
        p.push("solana");
        p.push("github");
        p.set_extension("md");

        assert_eq!(p.as_os_str(), OsStr::new("/home/hml/solana/github.md"));

        let p2: PathBuf = ["/home/hml", "solana", "github.md"].iter().collect();
        assert_eq!(p2.as_os_str(), OsStr::new("/home/hml/solana/github.md"));
        
    }

    //path iter
    {
        let mut p1 = Path::new("/tmp/foo/bar.txt").components();
        p1.next();
        p1.next();
        assert_eq!(Path::new("foo/bar.txt"), p1.as_path());

        let p2 = Path::new("/tmp/foo/bar.txt").components();
        for _component in p2 {
           // println!("{component:?}");
        }
        /* print:
            RootDir
            Normal("tmp")
            Normal("foo")
            Normal("bar.txt")
         */

        let p3 = Path::new("./tmp/foo/bar.txt");
        let vec_c3: Vec<_> = p3.components().map(|comp| comp.as_os_str()).collect();
        assert_eq!(&vec_c3, &[".", "tmp", "foo", "bar.txt"]);
    }
}

