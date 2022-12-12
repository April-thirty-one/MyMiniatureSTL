# ===================== 01_allocator =====================
jjaloc : 01_allocator/jjaloc.cpp
	g++ $^ -o jjaloc.out

clean :
	$(RM) *.o *.so *.a *.out