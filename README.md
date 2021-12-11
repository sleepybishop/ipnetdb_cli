# ipnetdb_cli

A quick and dirty cli for [ipnetdb](https://ipnetdb.com/) formed from the example code of [mmdb](https://github.com/klondi/mmdb) [^1] and altered to lookup up prefixes or asns.

### Building
```
git submodule init
git submodule update
make
make install
```
The tool and databases will be installed under `~/.local`

### Usage
Multiple keys can be given on the command line
```bash
[bishop@xps13 ipnetdb]$ ipnetdb 1.1.1.1 4.2.2.1 8.8.8.8 | jq -c '[.as, .as_name, .allocation]'
```
```json
[13335,"CLOUDFLARENET","1.1.1.0/24"]
[3356,"LEVEL3","4.0.0.0/9"]
[15169,"GOOGLE","8.0.0.0/9"]
```
```bash
[bishop@xps13 ipnetdb]$ ./ipnetdb 13335 3356 15169 | jq -c '[.as, .entity]'
```
```json
[13335,"Cloudflare, Inc."]
[3356,"Level 3 Parent, LLC"]
[15169,"Google LLC"]
```


[^1]: This small piece of glue is MIT licensed, however do note the [mmdb](https://github.com/klondi/mmdb) submodule is GPLv2 
