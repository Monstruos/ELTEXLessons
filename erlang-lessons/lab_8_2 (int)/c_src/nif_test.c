#include <erl_nif.h>

int fact(int inp)
{
    return inp > 0 ? inp * fact(inp - 1) : 1;
}

static ERL_NIF_TERM fact_nif(ErlNifEnv* env, int argc,
    const ERL_NIF_TERM argv[])
{
    int x, ret;
    if (!enif_get_int(env, argv[0], &x))
	   return enif_make_badarg(env);
    ret = fact(x);
    return enif_make_int(env, ret);
}

static ErlNifFunc nif_funcs[] = {
    {"fact", 1, fact_nif},
};

ERL_NIF_INIT(nif_test, nif_funcs, NULL, NULL, NULL, NULL)
