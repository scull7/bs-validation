open Jest;
/*
  Folketale/validation
  http://folktale.origamitower.com/api/v2.1.0/en/folktale.validation.html
*/
module FoldableArray = {
  type t('a) = array('a);
  let concat = (x, y) => Belt_Array.concat(x, y);
};

module V = Validation.Make_validation(FoldableArray);

module Language = {
  type t = {
    name: string,
    compiler: string,
  };

  let make = (name, compiler) => { name, compiler, };

  let toArray = (t) => [| t.name, t.compiler |];
};

describe("Validation.apply", () => {
  test("Should properly handle the Folktale language example", () => {
    V.(
      Success(Language.make)
      |> apply(Success("Rust"))
      |> apply(Success("rustc"))
      |> unsafeGet
      |> Language.toArray
      |> Expect.expect
      |> Expect.toBeSupersetOf([|"Rust", "rustc"|])
    );
  });
});
