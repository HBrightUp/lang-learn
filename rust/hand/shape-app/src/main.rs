use shape::{
    maths,
    plane::point::Point,
};

fn main() {
    println!("add: {}", maths::add(1,2));

    println!("sub: {}", maths::sub(10, 4));

    let p1 = Point::genrate_rand_point_by_f64();
    let p2 = Point::genrate_rand_point_by_i64();
    println!("f64 point: {}", &p1);
    println!("i64 point: {}", &p2);

    let p1_json = serde_json::to_string(&p1).unwrap();
    println!("{p1_json}");
}

