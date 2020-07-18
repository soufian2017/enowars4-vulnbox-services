using Microsoft.EntityFrameworkCore;
using Gamemaster.Models.Database;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

#pragma warning disable CS8618
namespace Gamemaster.Database
{
    public class GamemasterDbContext : DbContext
    {
        public DbSet<User> Users { get; set; }
        public DbSet<Session> Sessions { get; set; }
        public DbSet<SessionUserLink> SessionUserLinks { get; set; }
        public DbSet<Token> Tokens { get; set; }
        public DbSet<ChatMessage> ChatMessages { get; set; }



        public GamemasterDbContext(DbContextOptions<GamemasterDbContext> options) : base(options) { }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<SessionUserLink>()
                .HasKey(cul => new { cul.UserId, cul.SessionId });
            modelBuilder.Entity<User>().HasIndex(u => u.Name).IsUnique();
        }
    }
}
#pragma warning restore CS8618