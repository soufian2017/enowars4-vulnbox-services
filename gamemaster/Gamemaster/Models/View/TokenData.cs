using Gamemaster.Models.Database;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Security.Cryptography;
using System.Threading.Tasks;

namespace Gamemaster.Models.View
{
#pragma warning disable CS8618
    public class TokenData
    {
        public byte[] Icon { get; set; }
        public TokenData(Token t)
        {
            Icon = t.Icon;
        }
    }
#pragma warning disable CS8618
}
