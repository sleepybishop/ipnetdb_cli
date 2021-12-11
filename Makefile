CFLAGS:=-O2 -Immdb

DEPS=mmdb/mmdb.o

ipnetdb: main.o $(DEPS)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

scan:
	scan-build $(MAKE) clean ipnetdb

ubsan: CC=clang
ubsan: CFLAGS += -fsanitize=undefined,implicit-conversion,integer
ubsan: LDFLAGS += -lubsan
ubsan: clean ipnetdb
	./ipnetdb 50

.PHONY: clean
clean:
	$(RM) *.o ipnetdb $(DEPS)

latest.json:
	curl -sO https://ipnetdb.com/latest.json
ipnetdb_asn_latest.mmdb:
	curl -sO https://cdn.ipnetdb.net/ipnetdb_asn_latest.mmdb
ipnetdb_prefix_latest.mmdb:
	curl -sO https://cdn.ipnetdb.net/ipnetdb_prefix_latest.mmdb

install: ipnetdb ipnetdb_asn_latest.mmdb ipnetdb_prefix_latest.mmdb
	install -m 755 ipnetdb ~/.local/bin/
	install -m 755 ipnetdb_asn_latest.mmdb -Dt ~/.local/share/ipnetdb/latest/
	install -m 755 ipnetdb_prefix_latest.mmdb -Dt ~/.local/share/ipnetdb/latest/

