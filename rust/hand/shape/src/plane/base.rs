
#[derive(Debug, PartialEq)]
pub enum Quadrant {
    Origin,
    XAxis,
    YAxis,
    First,
    Second,
    Third,
    Fourth,
}

#[derive(Debug, PartialEq)]
pub enum LineRelation {
    None, 
    Parallel,
    Cross,
    Coincide,
}


pub trait Slope {
    fn slope(&self) -> f64;
}