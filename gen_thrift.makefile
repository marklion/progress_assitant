SRC_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
BACK_END_PATH=$(SRC_DIR)/pa_rpc/gen_code
FRONT_END_PATH=$(SRC_DIR)/pa_front/public/gen_code
AUTO_REPLY_PATH=$(SRC_DIR)/pa_script/gen_code


all:$(BACK_END_PATH) $(FRONT_END_PATH) $(AUTO_REPLY_PATH)

$(BACK_END_PATH):$(SRC_DIR)/idl.thrift
	rm -rf $(BACK_END_PATH)
	mkdir $(BACK_END_PATH)
	thrift -out $(BACK_END_PATH) --gen cpp:no_skeleton $^

$(FRONT_END_PATH):$(SRC_DIR)/idl.thrift
	rm -rf $(FRONT_END_PATH)
	mkdir $(FRONT_END_PATH)
	thrift -out $(FRONT_END_PATH) --gen js:es6 $^

$(AUTO_REPLY_PATH):$(SRC_DIR)/idl.thrift
	rm -rf $(AUTO_REPLY_PATH)
	mkdir $(AUTO_REPLY_PATH)
	thrift -out $(AUTO_REPLY_PATH) --gen py $^
