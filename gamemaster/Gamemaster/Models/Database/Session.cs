using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace Gamemaster.Models.Database
{
#pragma warning disable CS8618
    public class Session
    {
        public long Id { get; set; }
        public long OwnerId { get; set; }
        public User Owner { get; set; }
        public string Name { get; set; }
        public string Notes { get; set; }
        public DateTime Timestamp{ get; set; }
        [MaxLength(16)] public byte[] PasswordSalt { get; set; }
        [MaxLength(64)] public byte[] PasswordSha512Hash { get; set; }
        public List<SessionUserLink> Players { get; set; } = new List<SessionUserLink>();
    }
#pragma warning restore CS8618
}
