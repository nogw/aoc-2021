let read_lines = (name): list(string) => {
  let ic = open_in(name);
  let try_read = () =>
    try(Some(input_line(ic))) {
    | End_of_file => None
    };
  let rec loop = acc =>
    switch (try_read()) {
    | Some(s) => loop([s, ...acc])
    | None =>
      close_in(ic);
      List.rev(acc);
    };
  loop([]);
};

let rec pow = a =>
  fun
  | 0 => 1
  | 1 => a
  | n => {
      let b = pow(a, n / 2);
      b
      * b
      * (
        if (n mod 2 == 0) {
          1;
        } else {
          a;
        }
      );
    };

let expl = s => {
  let rec exp = (i, l) =>
    if (i < 0) {
      l;
    } else {
      exp(i - 1, [s.[i], ...l]);
    };
  exp(String.length(s) - 1, []);
};

let int_of_binary_string = s => {
  let rec process = (chars, acc, exp) =>
    switch (chars) {
    | [c, ...cs] =>
      process(
        cs,
        acc
        + pow(2, exp)
        * (
          if (c == '1') {
            1;
          } else {
            0;
          }
        ),
        exp + 1,
      )
    | [] => acc
    };
  process(List.rev(expl(s)), 0, 0);
};

let str_lines = read_lines("input.txt");
let num_digits = String.length(List.hd(str_lines));
let int_lines = List.map(int_of_binary_string, str_lines);

let count_ones = (list, pos) => {
  let one_or_zero = (exp, x) =>
    if (x land pow(2, exp) > 0) {
      1;
    } else {
      0;
    };
  List.fold_left((acc, x) => acc + x, 0, List.map(one_or_zero(pos), list));
};

let rec count_all_ones = (list, n_digits) =>
  switch (n_digits) {
  | (-1) => []
  | _ => [count_ones(list, n_digits), ...count_all_ones(list, n_digits - 1)]
  };

let new_results = count_all_ones(int_lines, num_digits - 1);

let one_or_zero = x =>
  if (x > List.length(int_lines) / 2) {
    1;
  } else {
    0;
  };
let condensed = List.map(one_or_zero, new_results);
let swapped =
  List.map(
    x =>
      if (x == 1) {
        0;
      } else {
        1;
      },
    condensed,
  );

let debinarize = binary_digit_list => {
  let rec process = (acc, l, exp) =>
    switch (l) {
    | [x, ...xs] => process(acc + x * pow(2, exp), xs, exp + 1)
    | [] => acc
    };
  process(0, List.rev(binary_digit_list), 0);
};

let epsilon = debinarize(condensed);
let gamma = debinarize(swapped);

print_int(epsilon * gamma);
