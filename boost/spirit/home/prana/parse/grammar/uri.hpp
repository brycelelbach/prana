template <typename Iterator, class Tag>
struct uri_grammar : qi::grammar<Iterator, uri_parts<Tag>()> {
    uri_grammar() : uri_grammar::base_type(start, "uri") {
        // gen-delims = ":" / "/" / "?" / "#" / "[" / "]" / "@"
        gen_delims %= qi::char_(":/?#[]@");
        // sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
        sub_delims %= qi::char_("!$&'()*+,;=");
        // reserved = gen-delims / sub-delims
        reserved %= gen_delims | sub_delims;
        // unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
        unreserved %= qi::alnum | qi::char_("-._~");
        // pct-encoded = "%" HEXDIG HEXDIG
        pct_encoded %= qi::char_("%") >> qi::repeat(2)[qi::xdigit];

        // pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
        pchar %= qi::raw[
            unreserved | pct_encoded | sub_delims | qi::char_(":@")
            ];

        // segment = *pchar
        segment %= qi::raw[*pchar];
        // segment-nz = 1*pchar
        segment_nz %= qi::raw[+pchar];
        // segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
        segment_nz_nc %= qi::raw[
            +(unreserved | pct_encoded | sub_delims | qi::char_("@"))
            ];
        // path-abempty  = *( "/" segment )
        path_abempty %= qi::raw[*(qi::char_("/") >> segment)];
        // path-absolute = "/" [ segment-nz *( "/" segment ) ]
        path_absolute %= qi::raw[
            qi::char_("/")
            >>  -(segment_nz >> *(qi::char_("/") >> segment))
            ];
        // path-rootless = segment-nz *( "/" segment )
        path_rootless %= qi::raw[
            segment_nz >> *(qi::char_("/") >> segment)
            ];
        // path-empty = 0<pchar>
        path_empty %= qi::eps;

        // scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
        scheme %= qi::alpha >> *(qi::alnum | qi::char_("+.-"));

        // user_info = *( unreserved / pct-encoded / sub-delims / ":" )
        user_info %= qi::raw[
            *(unreserved | pct_encoded | sub_delims | qi::char_(":"))
            ];

        // dec-octet = DIGIT / %x31-39 DIGIT / "1" 2DIGIT / "2" %x30-34 DIGIT / "25" %x30-35
        dec_octet %=
            !(qi::lit('0') >> qi::digit)
            >>  qi::raw[
                qi::uint_parser<boost::uint8_t, 10, 1, 3>()
                ];
        // IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
        ipv4address %= qi::raw[
            dec_octet >> qi::repeat(3)[qi::lit('.') >> dec_octet]
            ];
        // reg-name = *( unreserved / pct-encoded / sub-delims )
        reg_name %= qi::raw[
            *(unreserved | pct_encoded | sub_delims)
            ];
        // TODO, host = IP-literal / IPv4address / reg-name
        host %= ipv4address | reg_name;

        // query = *( pchar / "/" / "?" )
        query %= qi::raw[*(pchar | qi::char_("/?"))];
        // fragment = *( pchar / "/" / "?" )
        fragment %= qi::raw[*(pchar | qi::char_("/?"))];

        // hier-part = "//" authority path-abempty / path-absolute / path-rootless / path-empty
        // authority = [ userinfo "@" ] host [ ":" port ]
        hier_part %=
            (
                "//"
                >>  -(user_info >> '@')
                >>  host
                >>  -(':' >> qi::ushort_)
                // >>  -(':' >> *qi::digit)
                >>  path_abempty
                )
            |
            (
                qi::attr(optional<typename boost::network::string<Tag>::type>())
                >>  qi::attr(optional<typename boost::network::string<Tag>::type>())
                // >>  qi::attr(optional<typename boost::network::string<Tag>::type>())
                >>  qi::attr(optional<boost::uint16_t>())
                >>  (
                    path_absolute
                    |   path_rootless
                    |   path_empty
                    )
                );

        uri %=
            scheme >> ':'
                   >>  hier_part
                   >>  -('?' >> query)
                   >>  -('#' >> fragment);

        start %= uri.alias();
    }

    typedef typename string<Tag>::type string_type;

    qi::rule<Iterator, typename string_type::value_type()>
    gen_delims, sub_delims, reserved, unreserved;
    qi::rule<Iterator, string_type()>
    pct_encoded, pchar;

    qi::rule<Iterator, string_type()>
    segment, segment_nz, segment_nz_nc;
    qi::rule<Iterator, string_type()>
    path_abempty, path_absolute, path_rootless, path_empty;

    qi::rule<Iterator, string_type()>
    dec_octet, ipv4address, reg_name, host;

    qi::rule<Iterator, string_type()>
    scheme, user_info, query, fragment;

    qi::rule<Iterator, boost::fusion::tuple<
                           optional<string_type> &,
                           optional<string_type> &,
                           // optional<string_type> &,
                           optional<boost::uint16_t> &,
                           string_type &
                           >()> hier_part;

  // start rule of grammar
  qi::rule<Iterator, uri_parts<Tag>()> start;

  // actual uri parser
  qi::rule<Iterator, typename uri_parts_tuple<Tag>::type()> uri;
};

