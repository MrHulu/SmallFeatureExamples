# 🌐 Qt如何翻译JSON内容

## 📝 项目概述

这个项目展示了如何在Qt应用中实现JSON文件内容的多语言翻译功能。不同于官方示例中仅翻译界面上的静态文本，本项目重点演示如何将从JSON文件加载的动态内容进行翻译处理。

通过结合使用Qt的`QTranslator`和QML的`qsTranslate`函数，项目实现了中英文的无缝切换，为开发多语言应用提供了完整解决方案。

## ✨ 功能特点

- 📊 支持JSON数据的多语言翻译
- 🔄 实时语言切换而无需重启应用
- 🛠️ 与Qt标准翻译机制完全集成
- 📱 在QML界面中简单使用

## 🔑 关键实现点

### 1️⃣ JSON内容翻译的实现

在QML中使用`qsTranslate`函数翻译JSON内容：

```qml
// 翻译静态文本
text: qsTranslate("HULU", "高达")

// 动态翻译JSON内容 
text: "GD:" + qsTranslate("HULU", test.ts.GD)
text: "JW:" + qsTranslate("HULU", test.ts.JW)
text: "EFSF:" + qsTranslate("HULU", test.ts.EFSF)
```

**关键点**：
- `"HULU"`是翻译上下文（context）
- `test.ts.GD`是从JSON加载的文本内容
- 此方法将JSON内容作为翻译的源文本提供给翻译系统

### 2️⃣ 翻译文件加载

>必须要在代码里实现qm文件的加载！使用==engine.setUiLanguage==和==qApp->installTranslator==

```cpp
// 加载翻译文件
QMap<QString, QTranslator*> translators;
auto path = QString("%1/translations").arg(QCoreApplication::applicationDirPath());
QDir dir(path);
for (const auto& info : dir.entryInfoList({"*.qm"}, QDir::Files, QDir::Size)) {
    if (auto translator = new QTranslator(qApp);
        translator->load(info.absoluteFilePath())) {
            translators.insert(translator->language(), translator);
        }
}
// 中文是源语言，所以需要插入一个空翻译器
translators.insert("zh_CN", nullptr);

// 语言切换实现
auto currentLanguage = test.isChinese() ? "zh_CN" : "en_US";
engine.setUiLanguage(currentLanguage);
qApp->installTranslator(translators[currentLanguage]);
```

### 3️⃣ 动态切换语言

```cpp
QObject::connect(&test, &Test::isChineseChanged, &engine, [&]() {
    // 先清除所有现有翻译器
    const auto translator = translators[currentLanguage];
    qApp->removeTranslator(translator);
    
    // 重新加载翻译文件
    currentLanguage = test.isChinese() ? "zh_CN" : "en_US";
    qApp->installTranslator(translators[currentLanguage]);
    
    // 设置新的UI语言
    engine.setUiLanguage(currentLanguage);
    qDebug() << "更新翻译：" << (test.isChinese() ? "zh_CN" : "en_US");
});
```

### 4️⃣ CMake配置

>这里需要重点解释一下：
>
>1. **qt5_add_translation：** 是Qt5.15版本新增的函数，用于将.ts文件转换为.qm文件
>2. **add_custom_target：** 是CMake的函数，用于添加一个自定义目标，这里使用的目的就是为了生成.qm文件
>3. **add_custom_command：** 是CMake的函数，用于添加一个自定义命令，这里使用的目的就是为了将.qm文件复制到翻译目录
>4. **add_dependencies：** 是CMake的函数，用于添加一个依赖，这里使用的目的就是为了在编译时，先生成.qm文件，再编译项目

```cmake

# 翻译文件处理
set(TS_FILE ${CMAKE_SOURCE_DIR}/Info_en_US.ts)
qt5_add_translation(QM_FILES ${TS_FILE})
add_custom_target(qmfiles ALL DEPENDS ${QM_FILES})
add_custom_command(TARGET QtCppDemo DEPENDS qmfiles
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/translations"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QM_FILES} "${CMAKE_CURRENT_BINARY_DIR}/translations"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_dependencies(QtCppDemo qmfiles)
```

## 📦 必要组件

1. **翻译文件(.ts)**：
   - 包含所有需翻译的字符串（包括JSON中的内容）
   - 使用`lupdate`工具从源代码提取字符串（本实例用的是JSON文件，使用`lupdate`应该无法生成，所以需要**手动创建ts文件并手动填入内容**）
   - 使用`linguist`工具编辑翻译

2. **编译后的翻译文件(.qm)**：
   - 从.ts文件通过`lrelease`工具生成
   - **在CMake中使用`qt5_add_translation`（推荐使用）**

3. **翻译安装**：
   - 创建`QTranslator`对象
   - 加载.qm文件
   - 使用`qApp->installTranslator()`安装翻译器

4. **在QML中使用翻译**：
   - 使用`qsTranslate("上下文", "源文本")`翻译JSON内容


## 🔧 优化建议

1. **更灵活的语言选择** 🌍：
   - 当前实现只支持中英文切换，可扩展为支持更多语言
   - 可添加语言选择菜单而不仅是开关

2. **自动检测系统语言** 🔍：
   ```cpp
   // 获取系统区域设置
   QString locale = QLocale::system().name();
   // 根据系统语言设置初始语言
   ```

3. **使用资源系统管理翻译文件** 📁：
   - 将.qm文件嵌入到资源系统，避免依赖外部文件
   ```cmake
   qt5_add_translation(QM_FILES ${TS_FILES})
   # 将生成的qm文件添加到资源文件中
   qt5_add_resources(QRC_FILES translations.qrc)
   ```

4. **分离翻译上下文** 🧩：
   - 对界面文本和JSON内容使用不同的翻译上下文，便于管理

5. **缓存翻译结果** ⚡：
   - 对于频繁使用的JSON键值对，可以考虑缓存翻译结果以提高性能

## ⚠️ 注意事项

1. JSON内容必须作为源文本在.ts文件中存在
2. 更新JSON内容后需要**手动**更新翻译文件
3. 需确保翻译目录存在并且包含所有必要的翻译文件

## 💡 为什么选择这种方法？

这种方法的优点是统一了翻译机制，无需为JSON内容单独实现翻译逻辑。它使用Qt内置的翻译系统，维护成本低，且与Qt的其他部分集成良好。

## 🔗 相关链接

- [Qt Linguist手册](https://doc.qt.io/qt-5/linguist-manual.html)
- [QML国际化](https://doc.qt.io/qt-5/qtquick-internationalization.html)
- [Qt翻译工具](https://doc.qt.io/qt-5/internationalization.html) 