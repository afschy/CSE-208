#include <iostream>
#include <fstream>
#include <algorithm>
#define endl "\n"
#define INF 100000000

int compute_bandwidth(char** mat, const int r, const int c){
	int bandWidth = 0;
	for(int i=0; i<r; i++){
		for(int j=0; j<c; j++){
			if(mat[i][j] != 'X') continue;
			bandWidth = std::max(bandWidth, 1 + abs(i - j));
		}
	}
	return bandWidth;
}

void copy_matrix(char** dest, char** source, const int r, const int c){
	for(int i=0; i<r; i++){
		for(int j=0; j<c; j++){
			dest[i][j] = source[i][j];
		}
	}
}

void swap_rows(char** mat, const int r1, const int r2, const int n){
	for(int j=0; j<n; j++)
		std::swap(mat[r1][j], mat[r2][j]);
}

void swap_cols(char** mat, const int c1, const int c2, const int n){
	for(int i=0; i<n; i++)
		std::swap(mat[i][c1], mat[i][c2]);
}

void float_row_up(char** mat, const int target, const int start, const int n){
	int j = start, i = j-1;
	while(i >= target){
		swap_rows(mat, i, j, n);
		i--;
		j--;
	}
}

void float_row_down(char** mat, const int target, const int start, const int n){
	int j = start, i = j+1;
	while(i <= target){
		swap_rows(mat, i, j, n);
		i++;
		j++;
	}
}

void float_col_left(char** mat, const int target, const int start, const int n){
	int j = start, i = j-1;
	while(i >= target){
		swap_cols(mat, i, j, n);
		i--;
		j--;
	}
}

void float_col_right(char** mat, const int target, const int start, const int n){
	int j = start, i = j+1;
	while(i <= target){
		swap_cols(mat, i, j, n);
		i++;
		j++;
	}
}

int fixed_approx(char** mat, const int fixedRowCount, const int fixedColCount, const int n){
	int ans = 0;
	for(int i=0; i<fixedRowCount; i++){
		int count = 0;
		for(int j=fixedColCount; j<n; j++)
			if(mat[i][j] == 'X') count++;
		if(count)
			ans = std::max(ans, fixedColCount - i + count);
	}

	for(int j=0; j<fixedColCount; j++){
		int count = 0;
		for(int i=fixedRowCount; i<n; i++)
			if(mat[i][j] == 'X') count++;
		if(count)
			ans = std::max(ans, fixedRowCount - j + count);
	}

	ans = std::max(ans, compute_bandwidth(mat, fixedRowCount, fixedColCount));
	return ans;
}

int free_approx(char** mat, const int fixedRowCount, const int fixedColCount, const int n){
	int ans = 0;
	for(int i=fixedRowCount; i<n; i++){
		int count = 0;
		for(int j=0; j<n; j++)
			if(mat[i][j] == 'X') count++;
		ans = std::max(ans, (count+1)/2);
	}

	for(int j=fixedColCount; j<n; j++){
		int count = 0;
		for(int i=0; i<n; i++)
			if(mat[i][j] == 'X') count++;
		ans = std::max(ans, (count+1)/2);
	}

	return ans;
}

void reduce(char** currMat, const int fixedRowCount, const int fixedColCount, const int n, char** result, const int targetBound){
	int resultBandwidth = compute_bandwidth(result, n, n);
	if(resultBandwidth == targetBound) return;
	
	if(fixedRowCount == n-1 && fixedColCount == n-1){
		// int bandWidth = compute_bandwidth(currMat, n, n);
		if(compute_bandwidth(currMat, n, n) <= resultBandwidth)
			copy_matrix(result, currMat, n, n);
		return;
	}

	if(fixedRowCount == fixedColCount){
		int minBound = INF;
		int index = -1;

		for(int i=fixedColCount; i<n; i++){
			float_col_left(currMat, fixedColCount, i, n);
			int bound = std::max(fixed_approx(currMat, fixedRowCount, fixedColCount+1, n), free_approx(currMat, fixedRowCount, fixedColCount+1, n));
			if(bound <= minBound){
				minBound = bound;
				index = i;
			}
			float_col_right(currMat, i, fixedColCount, n);
		}

		if(minBound >= resultBandwidth) return;
		
		for(int i=index; i>=fixedColCount; i--){
			float_col_left(currMat, fixedColCount, i, n);
			int bound = std::max(fixed_approx(currMat, fixedRowCount, fixedColCount+1, n), free_approx(currMat, fixedRowCount, fixedColCount+1, n));
			if(bound == minBound)
				reduce(currMat, fixedRowCount, fixedColCount+1, n, result, targetBound);
			float_col_right(currMat, i, fixedColCount, n);
			if(bound==minBound && minBound >= compute_bandwidth(result, n, n)) break;
		}
	}

	else{
		int minBound = INF;
		int index = -1;

		for(int i=fixedRowCount; i<n; i++){
			float_row_up(currMat, fixedRowCount, i, n);
			int bound = std::max(fixed_approx(currMat, fixedRowCount+1, fixedColCount, n), free_approx(currMat, fixedRowCount+1, fixedColCount, n));
			if(bound <= minBound){
				minBound = bound;
				index = i;
			}
			float_row_down(currMat, i, fixedRowCount, n);
		}

		if(minBound >= resultBandwidth) return;

		for(int i=index; i>=fixedRowCount; i--){
			float_row_up(currMat, fixedRowCount, i, n);
			int bound = std::max(fixed_approx(currMat, fixedRowCount+1, fixedColCount, n), free_approx(currMat, fixedRowCount+1, fixedColCount, n));
			if(bound == minBound)
				reduce(currMat, fixedRowCount+1, fixedColCount, n, result, targetBound);
			float_row_down(currMat, i, fixedRowCount, n);
			if(bound==minBound && minBound >= compute_bandwidth(result, n, n)) break;
		}
	}
}

void print_matrix(char** mat, const int r, const int c){
	for(int i=0; i<r; i++){
		for(int j=0; j<c; j++)
			std::cout<<mat[i][j]<<" ";
		std::cout<<endl;
	}
}

int main(){
	std::ifstream inFile;
	inFile.open("in.txt");
	freopen("out.txt", "w", stdout);

	int n;
	inFile>>n;
	char** mat = new char*[n];
	char** result = new char*[n];
	for(int i=0; i<n; i++){
		mat[i] = new char[n];
		result[i] = new char[n];
	}
	
	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			inFile>>mat[i][j];
	inFile.close();

	copy_matrix(result, mat, n, n);
	reduce(mat, 0, 0, n, result, free_approx(mat, 0, 0, n));
	std::cout<<compute_bandwidth(result, n, n)<<endl;
	print_matrix(result, n, n);

	for(int i=0; i<n; i++){
		delete[] mat[i];
		delete[] result[i];
	}
	delete[] mat;
	delete[] result;

	return 0;
}