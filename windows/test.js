
const mouse = require('./build/Release/mouse.node')

let count = 0;

function click() {
    console.log('mouse.click()')
    mouse.click()
    count++
    if (count < 10) setTimeout(click, 100)
}

setTimeout(click, 3000)
