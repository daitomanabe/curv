// Copyright Doug Moen 2016-2017.
// Distributed under The MIT License.
// See accompanying file LICENSE.md or https://opensource.org/licenses/MIT

#include <curv/function.h>
#include <curv/exception.h>
#include <curv/context.h>
#include <curv/gl_context.h>

namespace curv {

const char Function::name[] = "function";

void
Function::print(std::ostream& out) const
{
    out << "<function>";
}

GL_Value
Function::gl_call_expr(Operation& arg, const Call_Phrase* call_phrase, GL_Frame& f)
const
{
    throw Exception(At_GL_Phrase(*call_phrase->function_, &f),
        "this function does not support the Geometry Compiler");
}

Value
Polyadic_Function::call(Value arg, Frame& f)
{
    if (nargs_ == 1) {
        f[0] = arg;
        return call(f);
    }
    auto list = arg.dycast<const List>();
    if (list && list->size() == nargs_) {
        for (size_t i = 0; i < list->size(); ++i)
            f[i] = (*list)[i];
        return call(f);
    } else {
        throw Exception(At_Phrase(*f.call_phrase->arg_, &f),
            stringify("function call argument is not a list of length ",nargs_));
    }
}

GL_Value
Polyadic_Function::gl_call_expr(
    Operation& arg, const Call_Phrase* call_phrase, GL_Frame& f)
const
{
    auto list = dynamic_cast<List_Expr*>(&arg);
    size_t nargs = (
        nargs_ == 1 ? 1
        : list ? list->size()
        : 1
    );
    if (nargs != nargs_) {
        throw Exception(At_GL_Phrase(*arg.source_, &f),
            "wrong number of arguments");
    }
    auto f2 = GL_Frame::make(nslots_, f.gl, &f, call_phrase);
    if (nargs == 1)
        (*f2)[0] = arg.gl_eval(f);
    else {
        for (size_t i = 0; i < list->size(); ++i)
            (*f2)[i] = (*list)[i]->gl_eval(f);
    }
    return gl_call(*f2);
}

GL_Value
Polyadic_Function::gl_call(GL_Frame& f) const
{
    throw Exception(At_GL_Frame(&f),
        "this function does not support the Geometry Compiler");
}

Value
Closure::call(Frame& f)
{
    f.nonlocal = &*nonlocal_;
    return expr_->eval(f);
}

GL_Value
Closure::gl_call(GL_Frame& f) const
{
    f.nonlocal = &*nonlocal_;
    return expr_->gl_eval(f);
}

void
Lambda::print(std::ostream& out) const
{
    out << "<lambda>";
}

} // namespace curv
