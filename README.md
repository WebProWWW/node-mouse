# 🖱️ mouse-addon

**Node.js Native Addon** для симуляции кликов мышью на **Windows** и **macOS**.

## 🛠️ Поддерживаемые платформы

| Платформа | Статус | Реализация |
| --- | --- | --- |
| Windows | ✅ | WinAPI (SendInput) |
| macOS | ✅ | Quartz Event Services (CGEventCreateMouseEvent) |

## 📁 Структура проекта

```arduino
.
├── macos
│   ├── binding.gyp     // Конфигурация node-gyp
│   ├── mouse.cpp       // Основной код C++
│   ├── package.json    // Скрипты и зависимости
│   └── test.js         // Пример использования
├── windows
│   ├── binding.gyp     // Конфигурация node-gyp
│   ├── mouse.cpp       // Основной код C++
│   ├── package.json    // Скрипты и зависимости
│   └── test.js         // Пример использования
├── .gitignore
└── README.md
```

## 📦 Установка

```bash
git clone https://github.com/webprowww/node-mouse.git

# macOS
cd mouseclick-addon/macos

# Windows
cd mouseclick-addon/windows

npm install
npx node-gyp configure
npx node-gyp build
```

Убедитесь, что у вас установлены следующие инструменты:

| Инструмент | Windows | macOS |
| --- | --- | --- |
| Node.js | ✅ | ✅ |
| Python 3 | ✅ | ✅ |
| node-gyp | ✅ | ✅ |
| Visual Studio Build Tools | ✅ | ❌ |
| Xcode (Command Line Tools) | ❌ | ✅ |

## 🚀 Быстрый старт

```javascript
// macOS
const mouse = require('macos\build\Release\mouse.node');

// Windows
const mouse = require('windows\build\Release\mouse.node');

console.log('Клик через 2 секунды...');

setTimeout(() => {
  mouse.click();
  console.log('Клик выполнен!');
}, 2000);
```

## ✨ Планы на будущее

- [ ] Поддержка правой кнопки мыши
- [ ] Клики по заданным координатам
- [ ] Эмуляция двойного клика
- [ ] Интеграция с виртуальными HID-устройствами

## Можно ли определить, что клик был программным?

Да, на Windows это можно сделать с помощью low-level-хука по флагу `LLMHF_INJECTED`.<br>На macOS определить программный клик гораздо сложнее.