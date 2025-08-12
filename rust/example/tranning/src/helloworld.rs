use::{
    chrono::{ Utc, Local, TimeZone, Duration, DateTime},
    std::collections::BTreeSet,
};

// Hello world
pub fn hello() -> &'static str {
    "Hello, world!"
}

// GigaSecond
pub fn after(start: DateTime<Utc>) -> DateTime<Utc> {
    start + Duration::seconds(1_000_000_000)
}

pub fn after_local(start: DateTime<Local>) -> DateTime<Local> {
    start + Duration::seconds(1_000_000_000)
}

// Leap
pub fn is_leap_year(year: i32) -> bool {
    let has_factor = |n| year % n == 0;
    has_factor(4) && (!has_factor(100) || has_factor(400))
}

// Raindrops
pub fn rain_drops( x: usize) -> String {
    let is_pling = |x| x % 3 == 0;
    let is_plang = |x| x % 5 == 0;
    let is_plong = |x| x %7 == 0;

    let mut output = String::new();
    if is_pling(x) {
        output.push_str("Pling");
    }

    if is_plang(x) {
        output.push_str("Plang");
    }

    if is_plong(x) {
        output.push_str("Plong");
    }

    if output.is_empty() {
        output.push_str(&format!("{}",x));
    }

    output
}

// Reverse String
pub fn reverse(input: &str) -> String {
    let mut output = String::with_capacity(input.len());
    output.extend(input.chars().rev());

    output
 }

 // Nth prime
pub fn nth(n: u32) -> u32 {
    if n == 0 {
        return 2;
    }

    let mut count = 0;
    let mut condidate =1;

    while count < n {
        condidate += 2;
        if is_prime(condidate) {
            count += 1;
        }
    }
    condidate
 }

 fn is_prime(n: u32) -> bool {
    let mut i = 3;
    while i * i < n + 1  {
        if n % i == 0 {
            return false;
        }
        i += 1;
    }

    true
 }

 // Difference Of Square
 pub fn difference(n:usize) -> usize {
    let sum_square = (n * (n + 1) / 2).pow(2);

    // fold
    let square_sum = (1..n+1).map( |x| x.pow(2)).fold( 0, |acc, x| acc + x);


    println!("sum_square: {}, square_sum: {:?} ", sum_square, square_sum);

    sum_square - square_sum
 }
 // Sum Of Multiples
 pub fn sum_of_multiples(limit: u32, factors: &[u32]) -> u32 {
    let mut multiples: BTreeSet<u32> = BTreeSet::new();

    for &f in factors {
        let mut multiplier = 2;
        let mut x = f;
        while x < limit {
            multiples.insert(x);
            x = f * multiplier;
            multiplier += 1;
        }
    }

    multiples.iter().sum()
 }

 // Pythagorean Triplet
 pub fn pythagorean() -> Option<u32> {
    for a in 1..1000 {
        for b in (a + 1)..(1000 - a) {
            let c = 1000 - (a + b);
            if a * a + b * b == c * c {
                return Some(a * b * c);
            }
        }
    }
    None
 }

 // Prime Factors
 pub fn factors(n: u64) -> Vec<u64> {
    let mut val = n;
    let mut out: Vec<u64> = vec![];
    let mut possible: u64 = 2;
    while val > 1 {
        while val % possible == 0 {
            out.push(possible);
            val /= possible;
        }
        possible += 1;
    }
    out
 }

 // Series
 pub fn series(digits: &str, len: usize) -> Vec<String> {
    match len {
        0 => vec!["".to_string(); digits.len() + 1],
        _ => digits
            .chars()
            .collect::<Vec<char>>()
            .windows(len)
            .map(|window| window.into_iter().collect::<String>())
            .collect::<Vec<String>>(),
    }
 }
 
 //Armstrong Numbers
 pub fn is_armstrong_number(num: u32) -> bool {
    let s = format!("{}", num);
    let l = s.len();
    s.chars()
        .map(|c| c.to_digit(10).unwrap().pow(l as u32))
        .sum::<u32>() == num
 }

 // Collatz Conjecture
 pub fn collatz_positive(n: u64) -> u64 {
    if n == 1 {
        0
    } else {
        1 + match n % 2 {
            0 => collatz_positive(n / 2),
            _ => collatz_positive(n * 3 + 1),
        }
    }
 }

#[cfg(test)] 
mod test{
    
    use super::*;

    #[test]
    fn test_hello() {
        let hello = hello();
        assert_eq!(hello, "Hello, world!");
    }

    #[test]
    fn test_leap_year() {
        assert!(is_leap_year(1996));
        assert!(!is_leap_year(1997));
        assert!(is_leap_year(1600));
        assert!(!is_leap_year(1800));
    }

    #[test]
    fn test_datetime() {

        let start_date = Utc.with_ymd_and_hms(2011, 4, 25, 0, 0, 0).unwrap();
        assert_eq!(after(start_date), Utc.with_ymd_and_hms(2043, 1, 1, 1, 46, 40).unwrap());
        
        let start_date = Utc.with_ymd_and_hms(2015, 1, 24, 23, 59, 59).unwrap();
        assert_eq!(after(start_date), Utc.with_ymd_and_hms(2046, 10, 3, 1, 46, 39).unwrap());

        let start_date = Local.with_ymd_and_hms(2015, 1, 24, 23, 59, 59).unwrap();
        assert_eq!(after_local(start_date), Local.with_ymd_and_hms(2046, 10, 3, 1, 46, 39).unwrap());
    }

    #[test]
    fn test_rain_drops() {
        assert_eq!(rain_drops(4), "4".to_string());
        assert_eq!(rain_drops(6), "Pling".to_string());
        assert_eq!(rain_drops(35), "PlangPlong".to_string());
        assert_eq!(rain_drops(88), "88".to_string());
        assert_eq!(rain_drops(70), "PlangPlong".to_string());
    }

    #[test]
    fn test_reverse() {
        process_reverse_case("", "");
        process_reverse_case("robot", "tobor");
        process_reverse_case("I'm hungry!", "!yrgnuh m'I");
        process_reverse_case("子猫", "猫子");

        #[cfg(feature = "grapheme")]    // Special character set processing
        process_reverse_case("uüu", "uüu");
    }

    #[test]
    fn test_nth() {
        assert_eq!(nth(0), 2);
        assert_eq!(nth(1), 3);
        assert_eq!(nth(5), 13);
        assert_eq!(nth(10000), 104743);
    }

    #[test]
    fn test_difference() {
        assert_eq!(170, difference(5));
        assert_eq!(25164150, difference(100));
    }

    #[test]
    fn test_sum_of_multiples() {
        assert_eq!(23, sum_of_multiples(10, &[3, 5]));
        assert_eq!(3, sum_of_multiples(4, &[3, 5]));
        assert_eq!(2203160, sum_of_multiples(10000, &[43, 47]));
        assert_eq!(4419, sum_of_multiples(150, &[5, 6, 8]));
    }

    fn process_reverse_case(input: &str, expected: &str) {
        assert_eq!(reverse(input), expected)
    }

    #[test]
    fn test_pythagorean() {
        assert_eq!(pythagorean(), Some(31875000));
    }

    #[test]
    fn test_factors() {
        assert_eq!(factors(8), vec![2, 2, 2]);
        assert_eq!(factors(901255), vec![5, 17, 23, 461]);
        assert_eq!(factors(93819012551), vec![11, 9539, 894119]);
    }

    #[test]
    fn test_series() {
        let expected = vec!["".to_string(); 6];
        assert_eq!(series("92017", 0), expected);

        let expected = vec![
            "92".to_string(),
            "20".to_string(),
            "01".to_string(),
            "17".to_string(),
        ];
        assert_eq!(series("92017", 2), expected);
    }
    
    #[test]
    fn test_is_armstrong_number() {
        assert!(is_armstrong_number(9474));
        assert!(!is_armstrong_number(100));
        assert!(is_armstrong_number(153));
        assert!(is_armstrong_number(9926315));
    }

    #[test]
    fn test_12() {
        assert_eq!(9, collatz_positive(12));
        assert_eq!(4, collatz_positive(16));
        assert_eq!(152, collatz_positive(1000000));
    }

    
}