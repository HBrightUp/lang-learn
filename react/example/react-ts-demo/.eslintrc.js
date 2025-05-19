module.exports = {
    "env":{
        "browser":true,
        "es2024": true
    },
    "extends": [
        "eslint:recommended",
        "plugin:prettier/recommended"
    ],
    "parser": "@typescript-eslint/parser",
    "parserOptions": {
      "ecmaVersion": "latest",
      "sourceType": "module"
    },
    "overrides": [
    ],
    "rules":{
        "semi": ["error", "always"],
        "quotes": ["error", "double"]
    }
}