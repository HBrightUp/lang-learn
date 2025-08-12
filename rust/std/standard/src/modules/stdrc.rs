use std::{
    rc::{
        Rc,Weak,
    },
    cell::{
        RefCell,
    },
};

struct Owner{
    name: String,
    gadgets:  RefCell<Vec<Weak<Gadget>>>,  // 定义 Weak<Gadget>
}

struct Gadget{
    id: u64,
    owner: Rc<Owner>,
}

pub fn stdrc() {

    //init
    {
        let r1 = Rc::new(vec![1, 2, 3, 4, 5, 6]);
        let r2 = r1.clone();
        let r3 = Rc::clone(&r1);

        assert_eq!(r1, r2);
        assert_eq!(r1, r3);
    }

    //Example: Preventing Circular References    
    {
        let owner = Rc::new(Owner{
            name: "Gadget".to_string(),
            gadgets: RefCell::new(vec![]),  
        });

        let gadget_1 = Rc::new(
            Gadget {
                id: 1u64,
                owner: owner.clone(),
            }
        );

        let gadget_2 = Rc::new(
            Gadget {
                id: 2u64,
                owner: owner.clone(),
            }
        );
        {
            let mut  gadgets = owner.gadgets.borrow_mut();
            gadgets.push(Rc::downgrade(&gadget_1));
            gadgets.push(Rc::downgrade(&gadget_2));

            for gadget_weak in gadgets.iter() {
                let gadget = gadget_weak.upgrade().unwrap();
                assert_eq!(Rc::weak_count(&gadget), 1);
                println!("gadget: id: {}, name: {}", gadget.id, gadget.owner.name);
            }
        }

        // Cannot drop when there is borrow or borrow_mut in the same scope
        let ref_owner_1  = owner.clone();

        assert_eq!(Rc::strong_count(&ref_owner_1), 4);
        drop(owner);
        assert_eq!(Rc::strong_count(&ref_owner_1), 3);

        assert_eq!(gadget_1.owner.name, "Gadget".to_string());
        assert_eq!(gadget_2.owner.name, "Gadget".to_string());

        
    }

}

