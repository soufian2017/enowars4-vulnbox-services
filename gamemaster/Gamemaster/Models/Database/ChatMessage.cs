using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Gamemaster.Models.Database
{
#pragma warning disable CS8618
    public class ChatMessage
    {
        public long Id { get; set; }
        public User Sender { get; set; }
        public long SenderId { get; set; }
        public Session SessionContext { get; set; }
        public long SessionContextId { get; set; }
        public string Content { get; set; }
        public DateTime Timestamp { get; set; }
    }
#pragma warning restore CS8618
}
