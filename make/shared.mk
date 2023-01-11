.PHONY: create_dirs copy_res copy_credits clean 

SRC_DIR += src
OUT += $(BIN_DIR)/fb
WARNING_FLAGS += -Wall -Wextra
CFLAGS += $(WARNING_FLAGS) -g
LDFLAGS += $(WARNING_FLAGS)
INCFLAGS += -Isrc
SRC += $(wildcard $(addsuffix /*.c, $(SRC_DIR)))
OBJ += $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

create_dirs:
	@echo -e "\e[32mCreating directories...\e[0m" 
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

copy_res:
	@echo -e "\e[32mCopying resources...\e[0m" 
	@rm -rf $(BIN_DIR)/res
	@cp -r ./res $(BIN_DIR)

copy_credits:
	@echo -e "\e[32mCopying credits.txt...\e[0m"
	@cp -r ./credits.txt $(BIN_DIR)

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)