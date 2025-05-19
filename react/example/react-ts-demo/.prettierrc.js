module.exports = {
    // 箭头函数只有一个参数里可以忽略括号;
    arrowParens: 'avoid',

    // 括号内部不要出现空格;
    bracketSpacing: false ,

    // 行结束符使用 unix 格式;
    endOfLine: 'lf',
    
    // 行宽;
    printWidth: 180,

    //换行方式
    proseWrap: 'preserve',

    // 分号;
    semi: true,

    // 使用单引号表示字符串;
    singleQuote: false,

    // tab 缩进间距;
    tabWidth: 2,

    // 是否雇用 tab 缩进;
    useTabs: false,

    // 后置读号，多行对象，数组在最后一行增加读号
    trailingComma: 'es5',
    parser: 'typescript',

}