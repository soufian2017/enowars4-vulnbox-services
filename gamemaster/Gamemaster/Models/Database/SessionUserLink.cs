using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Gamemaster.Models.Database
{
#pragma warning disable CS8618
    public class SessionUserLink
    {
        public long SessionId { get; set; }
        public Session Session { get; set; }
        public long UserId { get; set; }
        public User User { get; set; }
    }
#pragma warning restore CS8618
}
