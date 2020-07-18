using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Design;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Gamemaster.Database
{
    public class GamemasterDbContextFactory : IDesignTimeDbContextFactory<GamemasterDbContext>
    {
        public static string PostgresDomain => Environment.GetEnvironmentVariable("DATABASE_DOMAIN") ?? "localhost";
        public static string PostgresConnectionString => $@"Server={PostgresDomain};Port=5432;Database=GamemasterDatabase;User Id=docker;Password=docker;Timeout=15;SslMode=Disable";
        public GamemasterDbContext CreateDbContext(string[] args)
        {
            var optionsBuilder = new DbContextOptionsBuilder<GamemasterDbContext>();
            optionsBuilder.UseNpgsql(PostgresConnectionString);
            return new GamemasterDbContext(optionsBuilder.Options);
        }
    }
}