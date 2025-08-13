#include<iostream>
#include<map>

// the base class for all products.
template <class ProductType_t>
class IProductRegistrar
{
   public:
      virtual ProductType_t *CreateProduct() = 0;

   protected:
      IProductRegistrar() {}
      virtual ~IProductRegistrar() {}

   private:

      IProductRegistrar(const IProductRegistrar &);
      const IProductRegistrar &operator=(const IProductRegistrar &);
};

template <class ProductType_t>
class ProductFactory
{
public:
   static ProductFactory<ProductType_t> &Instance()
   {
      static ProductFactory<ProductType_t> instance;
      return instance;
   }

   void RegisterProduct(IProductRegistrar<ProductType_t> *registrar, std::string name)
   {
      m_ProductRegistry[name] = registrar;
   }

   ProductType_t *GetProduct(std::string name)
   {
      if (m_ProductRegistry.find(name) != m_ProductRegistry.end())
      {
         return m_ProductRegistry[name]->CreateProduct();
      }

      return nullptr;
   }

private:

   ProductFactory() {}
   ~ProductFactory() {}

   ProductFactory(const ProductFactory &);
   const ProductFactory &operator=(const ProductFactory &);

private:
   std::map<std::string, IProductRegistrar<ProductType_t> *> m_ProductRegistry;
};

// Register your product for here/
template <class ProductType_t, class ProductImpl_t>
class ProductRegistrar : public IProductRegistrar<ProductType_t>
{
   public:
      explicit ProductRegistrar(std::string name)
      {
         ProductFactory<ProductType_t>::Instance().RegisterProduct(this, name);
      }

      ProductType_t *CreateProduct()
      {
         return new ProductImpl_t();
      }
};

// Base class of product-1
class Shoes
{
   public:
      virtual void Show() = 0;
      virtual ~Shoes() {}
};

// Specific products
class NiKeShoes : public Shoes
{
   public:
      void Show()
      {
         std::cout << "NiKeShoes show" << std::endl;
      }
      virtual ~NiKeShoes() { std::cout << "NiKeShoes destructor call" << std::endl;}
};

// Base class of product-2
class Clothe
{
   public:
      virtual void Show() = 0;
      virtual ~Clothe() {}
   };

// Specific products
class UniqloClothe : public Clothe
{
   public:
      void Show()
      {
         std::cout << "UniqloClothe show" << std::endl;
      }
      virtual ~UniqloClothe() { std::cout << "UniqloClothe destructor call" << std::endl;}
};