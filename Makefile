# Имя выходного файла программы
PROGNAME = shootemup
# Выделяем из .cpp файлов в src/ чистый список имён модулей без путей и расширений
MODULES = $(patsubst %.cpp,%,$(notdir $(wildcard src/*.cpp)))
# Подготоваливаем списки модулей по типам добавляя имя подпапок впереди и расширения в конце
MODULES_CPP = $(addsuffix .cpp,$(addprefix src/,$(MODULES)))
MODULES_O = $(addsuffix .o,$(addprefix out/,$(MODULES)))
MODULES_DEP = $(addsuffix .dep,$(addprefix out/,$(MODULES)))

$(PROGNAME).gba: out/$(PROGNAME).elf
	objcopy -O binary out/$(PROGNAME).elf $(PROGNAME).gba
	$(DEVKITPRO)/tools/bin/gbafix $(PROGNAME).gba

out/$(PROGNAME).elf: $(MODULES_O)
	$(CXX) $(PARAMS) $(MODULES_O) -o out/$(PROGNAME).elf

out/%.o: src/%.cpp
	$(CXX) $(PARAMS) -o $@ -c $<

dep: $(MODULES_DEP)

out/%.dep: src/%.cpp
	$(CXX) -MT $(patsubst %.dep,%.o,$@) -MM $(PARAMS) $< > $@

-include $(MODULES_DEP)
