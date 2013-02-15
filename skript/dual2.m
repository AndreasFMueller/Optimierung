
A = [
  12, 18, 28,  0,  0,  0;
  -1,  1,  4, -4,  0,  2;
   3,  3,  1,  0, -1,  3
]

function B = pivot(A, i, j)
	B = A;
	B(i,:) = B(i,:) / B(i,j);
	for k = 1:size(B)(1)
		if (k != i)
			B(k,:) = B(k,:) - B(k,j) * B(i,:);
		endif
	endfor
endfunction;

A1 = pivot(A, 2,1)

A2 = pivot(A1, 3,3)
13 * A2

A3 = pivot(A2, 2, 2)
11 * A3
