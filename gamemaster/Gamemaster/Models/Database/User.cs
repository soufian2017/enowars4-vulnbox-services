using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace Gamemaster.Models.Database
{
#pragma warning disable CS8618
    public class User
    {
        public long Id { get; set; }
        public string Name { get; set; }
        public string Email { get; set; }
        [MaxLength(16)] public byte[] PasswordSalt { get; set; }
        [MaxLength(64)] public byte[] PasswordSha512Hash { get; set; }
        public List<SessionUserLink> Sessions { get; set; } = new List<SessionUserLink>();
        public List<Token> Tokens { get; set; } = new List<Token>();
    }
#pragma warning restore CS8618
}