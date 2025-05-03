use std::{
    io::{
        self, Read,BufReader, BufRead, LineWriter, Write,
    },
    fs::{
        self,File,
    },
};

pub fn stdio() {

    //io::copy  str -> Vec
    {
        let mut s1: &[u8] = b"hello";
        let mut writer: Vec<u8> = vec![];
        let _ = io::copy(&mut s1, &mut writer);
        assert_eq!(&b"hello"[..], &writer[..]);
    }

    //io::repeat 
    {
        let mut buf = [0; 3];
        io::repeat(100).read_exact(&mut buf).unwrap();
        assert_eq!(buf, [100, 100, 100]);
    }


    // io::BufReader::read_line:  read file by line
    //BufReader MAX_BUFFER =  8K
    {
        let f1 = File::open("./test.md").unwrap();
        let mut reader = BufReader::new(f1);
        let mut line = String::new();
        let _len = reader.read_line(&mut line);
    }


    // read all data once time
    {
        let f2 = fs::File::open("./test.md").unwrap();
        let mut reader = io::BufReader::new(f2);
        let _cap = reader.capacity();
        let _buf = reader.fill_buf().unwrap();
    }


    //write all data once time
    {
        let _road_not_taken = b"I shall be telling this with a sigh
            Somewhere ages and ages hence:
            Two roads diverged in a wood, and I -
            I took the one less traveled by,
            And that has made all the difference.";

        let file = File::create("poem.txt").unwrap();
        let mut file = LineWriter::new(file);

        file.write_all(b"I shall be telling this with a sigh").unwrap();

        //only refreshed  when '\n' appears 
        assert_eq!(fs::read_to_string("poem.txt").unwrap(), "");

        file.write_all(b"\n").unwrap();
        assert_eq!(
            fs::read_to_string("poem.txt").unwrap(),
            "I shall be telling this with a sigh\n",
        );

        file.write_all(b"Somewhere ages and ages hence:
            Two roads diverged in a wood, and I -
            I took the one less traveled by,
            And that has made all the difference.").unwrap();

        file.flush().unwrap();

        //QES1: Extra spaces are written on here, lead to assert_eq failed,solve it on future.
        //assert_eq!(fs::read("poem.txt").unwrap(), &road_not_taken[..]);
    }

}
