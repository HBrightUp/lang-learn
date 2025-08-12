use rand::prelude::*;

use log::info;
use rand::Rng;

pub fn enter() {
    random_api();
    
 
}

fn random_api() {
    let mut rng = rand::rng();
    const LEN : usize = 6;

    let mut rand_u8 = vec![];
    let mut rand_alphanumber = vec![];

    loop {
        let num: u8 = rng.random::<u8>();
        rand_u8.push(num);

        // Randomly generate letters or numbers
        let alphanumber = rng.sample(rand::distr::Alphanumeric);
        rand_alphanumber.push(alphanumber);

        if rand_u8.len() == LEN {
            break;
        }
    }
    
    info!("rand for u8: {:? }", rand_u8);
    
    for data in rand_alphanumber {
        info!("rand for alpha or number: '{:?}'", data as char);
    }


    let mut nums: Vec<i32> = (1..10).collect();

     // Pick a random element from array
    let rand_num = nums.choose(&mut rng);
    info!("rand num: {:?}", rand_num);

    //Generate and shuffle a sequence
    nums.shuffle(& mut rng);
    info!("nums: {:?}", nums);

    let mut arr = [0i8;20];
    rand::fill(&mut arr[..]);
    info!("arr: {:?}", arr);

    let words: Vec<&str> = "Mary had a little lamb".split(' ').collect();
    info!("word of random: {}", words[rand::random_range(..words.len())]);

    println!("{}", rand::random_ratio(2, 3));
}



#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_rand() {
        info!("coming from");
    }
}