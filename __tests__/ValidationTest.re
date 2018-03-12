open Jest;

module FoldableArray = {
  type t('a) = array('a);
  let concat = (x, y) => Belt_Array.concat(x, y);
};

module V = Validation.Make_validation(FoldableArray);

describe("Validation", () => {

  describe("Functor", () => {
    test("map should equal application", () => {
      let f = x => x + 1;
      let expected = V.make(f(1));
      let actual = V.make(1) |> V.map(f);
      Expect.expect(actual) |> Expect.toEqual(expected)
    });

    test("map should not modify a failure", () => {
      let f = x => x + 1;
      let expected = V.Failure([|1|]);
      let actual = V.Failure([|1|]) |> V.map(f);
      Expect.expect(actual) |> Expect.toEqual(expected)
    });
  });

  describe("Applicative", () => {
    test("make (of)", () => {
      let expected = V.Success(1);
      let actual = V.make(1);
      Expect.expect(actual) |> Expect.toEqual(expected)
    });

    test("Success#apply", () => {
      let f = (x, y) => x + y;
      let expected = V.Success(7);
      let actual =
        V.make(f)
        |> V.apply(V.Success(3))
        |> V.apply(V.Success(4));
      Expect.expect(actual) |> Expect.toEqual(expected)
    });

    test("Success/Failure#apply", () => {
      let f = (x, y) => x + y;
      let expected = V.Failure([|7|]);
      let actual =
        V.make(f)
        |> V.apply(V.Success(3))
        |> V.apply(V.Failure([|7|]));
      Expect.expect(actual) |> Expect.toEqual(expected)
    });

    test("Failure#apply", () => {
      let f = (x, y) => x + y;
      let expected = V.Failure([|7, 8|]);
      let actual =
        V.make(f)
        |> V.apply(V.Failure([|7|]))
        |> V.apply(V.Failure([|8|]));
      Expect.expect(actual) |> Expect.toEqual(expected)
    });
  });

  describe("Folds", () => {
    test("mapFailure should map over a failure", () => {
      let f = fun
        | [|x|] => [|x + 10|]
        | _ => failwith("unexpected_input");
      let expected = V.Failure([|11|]);
      let actual = V.Failure([|1|]) |> V.mapFailure(f);
      Expect.expect(actual) |> Expect.toEqual(expected)
    });
  });
});
