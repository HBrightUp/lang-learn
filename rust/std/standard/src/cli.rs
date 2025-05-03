

use crate::std::{
    Modules, PrimitiveTypes,
};
use crate::modules::*;
use crate::primitive_types::*;

 pub fn execute_modules(module: Modules) {
    match module {

        Modules::StdAny => {
            stdany::stdany();
        },
        Modules::StdArray => {
            stdarray::stdarray();
        },
        Modules::StdCell => {
            stdcell::stdcell();
        },
        Modules::StdCmp => {
            stdcmp::stdcmp();
        },
        Modules::StdCollections => {
            stdcollections::stdcollections();
        },
        Modules::StdConvert => {
            stdconvert::stdconvert();
        },
        Modules::StdEnv => {
            stdenv::stdenv();
        },
        Modules::StdFs => {
            stdfs::stdfs();
        }
        Modules::StdHash => {
            stdhash::stdhash();
        },
        Modules::StdIo => {
            stdio::stdio();
        },
        Modules::StdIter => {
            stditer::stditer();
        },
        Modules::StdMem => {
            stdmem::stdmem();
        }
        Modules::StdNet => {
            stdnet::stdnet();
        },
        Modules::StdNum => {
            stdnum::stdnum();
        },
        Modules::StdOps => {
            stdops::stdops();
        },
        Modules::StdPath => {
            stdpath::stdpath();
        },
        Modules::StdPtr => {
            stdptr::stdptr();
        },
        Modules::StdPin => {
            stdpin::stdpin();
        },
        Modules::StdRc => {
            stdrc::stdrc();
        },
        Modules::StdSlice => {
            stdslice::stdslice();
        },
        Modules::StdStr => {
            stdstr::stdstr();
        },
        Modules::StdString => {
            stdstring::stdstring();
        },
        Modules::StdThread => {
            stdthread::stdthread();
        },
        Modules::StdBoxed => {
            stdboxed::stdboxed();
        },
        Modules::StdOption => {
            stdoption::stdoption();
        },
        Modules::StdAlloc => {
            stdalloc::stdalloc();
        },
        Modules::StdArc => {
            stdarc::stdarc();
        }
    }
}

pub fn execute_primitive_types(primitive_types: PrimitiveTypes) {
    match primitive_types {
        PrimitiveTypes::Pointer => {
            pointer::stdpointer();
        }
    }
}

