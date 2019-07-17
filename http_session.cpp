#include "http_session.hpp"

std::istream &snet::http_session::read_until_crlf(std::istream &is, std::string &opt)
{
    // Ref: https://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
    // Modify a bit to make it only stop at CRLF and EOF
    opt.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry setry(is, true);
    std::streambuf* sbuf = is.rdbuf();

    for(;;) {
        int c = sbuf->sbumpc();
        switch (c) {
            case '\r':
                if(sbuf->sgetc() == '\n')
                    sbuf->sbumpc();
                return is;
            case std::streambuf::traits_type::eof():
                // Also handle the case when the last line has no line ending
                if(opt.empty())
                    is.setstate(std::ios::eofbit);
                return is;
            default:
                opt += (char)c;
        }
    }
}
