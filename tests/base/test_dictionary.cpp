#include <assert.hpp>
#include <base/dictionary.hpp>

int main() {
	base::dictionary d;
	d["abc"] = std::string("1");
	ASSERT(d.contains("abc"))
	ASSERT(d.get<std::string>("abc") == "1")
	d.put("abc", std::string("test"));
	ASSERT(d.get<std::string>("abc", "") == "test")
	ASSERT(d.get<std::string>("a", "test") == "test")
	base::dictionary e;
	e = d;
	d.put("abc", 5);
	ASSERT(e.get<std::string>("abc") == "test");
	ASSERT(d.get<int>("abc") == 5)
	return 0;
}