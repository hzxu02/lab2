use std::io;

// 사용자로부터 행렬의 크기(행, 열)를 입력받는 함수
fn get_dimensions() -> (usize, usize) {
    println!("행렬의 크기를 입력하세요 (예: 3 4):");
    let mut input = String::new();
    io::stdin().read_line(&mut input)
        .expect("입력을 읽는 데 실패했습니다.");

    let dims: Vec<usize> = input
        .trim()
        .split_whitespace()
        .map(|s| s.parse().expect("숫자를 입력하세요."))
        .collect();

    if dims.len() != 2 {
        panic!("정확히 두 개의 숫자를 입력해야 합니다.");
    }
    (dims[0], dims[1])
}

// 사용자로부터 행렬의 원소를 입력받아 Vec<Vec<i32>>를 생성하는 함수
// 이 함수에서 Vec<T>를 사용한 동적 할당이 이루어짐.
fn read_matrix(rows: usize, cols: usize) -> Vec<Vec<i32>> {
    println!("{}x{} 행렬의 원소를 입력하세요 (한 줄에 한 행씩, 공백 구분):", rows, cols);
    
    // Vec<T>를 사용하여 외부 벡터(행)를 동적으로 생성
    // with_capacity는 미리 공간을 확보하지만, Vec::new()를 써도 됨
    let mut matrix: Vec<Vec<i32>> = Vec::with_capacity(rows);

    for _ in 0..rows {
        let mut input = String::new();
        io::stdin().read_line(&mut input)
            .expect("입력을 읽는 데 실패했습니다.");

        // 각 행을 읽어 Vec<i32>로 변환
        // collect()는 필요한 만큼의 메모리를 동적으로 할당하여 내부 벡터(열)를 생성합니다.
        let row: Vec<i32> = input
            .trim()
            .split_whitespace()
            .map(|s| s.parse().expect("숫자를 입력하세요."))
            .collect();

        if row.len() != cols {
            panic!("입력된 열의 개수가 지정된 개수({})와 다릅니다.", cols);
        }
        
        // 동적으로 생성된 행(row)을 외부 벡터(matrix)에 추가
        matrix.push(row);
    }
    matrix
}

// 두 행렬을 더하는 함수
fn add_matrices(a: &Vec<Vec<i32>>, b: &Vec<Vec<i32>>) -> Vec<Vec<i32>> {
    let rows = a.len();
    let cols = a[0].len();

    // 결과를 저장할 Vec<Vec<i32>>를 동적으로 생성 (0으로 초기화)
    let mut result = vec![vec![0; cols]; rows];

    for i in 0..rows {
        for j in 0..cols {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    result
}

// 행렬을 출력하는 함수
fn print_matrix(matrix: &Vec<Vec<i32>>) {
    for row in matrix {
        // &val을 사용하여 값에 접근
        for &val in row {
            print!("{:>4} ", val); // 4칸에 맞춰 우측 정렬
        }
        println!();
    }
}

fn main() {
    // --- 첫 번째 행렬 입력 ---
    println!("--- 첫 번째 행렬 ---");
    let (rows1, cols1) = get_dimensions();
    let matrix1 = read_matrix(rows1, cols1);

    // --- 두 번째 행렬 입력 ---
    println!("\n--- 두 번째 행렬 ---");
    let (rows2, cols2) = get_dimensions();

    // 두 행렬의 크기가 다르면 덧셈 불가
    if rows1 != rows2 || cols1 != cols2 {
        println!("오류: 두 행렬의 크기가 같아야 덧셈이 가능합니다.");
        return;
    }
    
    let matrix2 = read_matrix(rows2, cols2);

    // --- 행렬 덧셈 ---
    let result_matrix = add_matrices(&matrix1, &matrix2);

    // --- 결과 출력 ---
    println!("\n--- 덧셈 결과 ---");
    print_matrix(&result_matrix);
}
