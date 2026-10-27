build: tiny_a tiny_b tiny_c

tiny_%: tiny_%.c
	gcc -Wall $< -o $@

clean:
	rm -f tiny_a tiny_b tiny_c out
