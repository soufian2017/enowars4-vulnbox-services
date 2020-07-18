using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using Gamemaster.Database;
using Gamemaster.Models.Database;

namespace Gamemaster.Controllers
{
    [Route("api/[controller]/[action]")]
    [ApiController]
    public class GameSessionController : Controller
    {
        private readonly ILogger<GameSessionController> Logger;
        private readonly IGamemasterDb Db;

        public GameSessionController(ILogger<GameSessionController> logger, IGamemasterDb db)
        {
            Logger = logger;
            Db = db;
        }
        [HttpPost]
        public async Task<IActionResult> Create([FromForm]string name, [FromForm]string notes, [FromForm]string password)
        {
            try
            {
                var username = HttpContext.User.Identity.Name;
                if (username == null) return Forbid();
                var owner = await Db.GetUser(username);
                if (owner == null) return Forbid();
                var session = await Db.InsertSession(name, notes, owner, password);
                return Json(session);
            }
            catch (Exception e)
            {
                Logger.LogError($"{nameof(Create)} failed: {e.Message}");
                return Forbid();
            }
        }

        [HttpPost]
        public async Task<IActionResult> GetInfo([FromForm]long id)
        {
            try
            {
                var username = HttpContext.User.Identity.Name;
                if (username == null)
                    throw new Exception("No Login Cookie found");
                var user = await Db.GetUser(username);
                if (user == null)
                    throw new Exception("User of session not found");
                var session = await Db.GetSession(id, user.Id);
                if (session == null)
                    throw new Exception("Session not found");
                return Json(session);
            }
            catch (Exception e)
            {
                Logger.LogError($"{nameof(GetInfo)} failed: {e.Message}");
                return Forbid();
            }
        }
        [HttpGet]
        public async Task<IActionResult> List()
        {
            try
            {
                var username = HttpContext.User.Identity.Name;
                var user = await Db.GetUser(username);
                var sessions = await Db.GetSessions(user.Id);
                foreach (var s in sessions)
                {
                    s.PasswordSalt = Array.Empty<byte>();
                    s.PasswordSha512Hash = Array.Empty<byte>();
                    s.Notes = string.Empty;                                       //Don't leak the flags!
                }
                return Json(sessions);
            }
            catch (Exception e)
            {
                Logger.LogError($"{nameof(List)} failed: {e.Message}");
                return Forbid();
            }
        }
        [HttpGet]
        public async Task<IActionResult> ListRecent(int skip, int take)
        {
            try
            {
                var sessions = await Db.GetRecentSessions(skip, take);
                return Json(sessions);
            }
            catch (Exception e)
            {
                Logger.LogError($"{nameof(ListRecent)} failed: {e.Message}");
                return Forbid();
            }
        }
        [HttpPost]
        public async Task<IActionResult> AddUser([FromForm] int sessionid,[FromForm] string username)
        {
            try
            {
                var currentusername = HttpContext.User.Identity.Name;
                if (currentusername == null)
                {
                    throw new System.Exception($"User not logged in");
                }
                var currentuser = await Db.GetUser(currentusername);
                if (!(currentuser is User _))
                {
                    throw new System.Exception($"No user called {currentusername} found");
                }
                var adduser = await Db.GetUser(username);
                if (!(adduser is User _))
                {
                    throw new System.Exception($"No user called {currentusername} found");
                }
                var session = await Db.GetSession(sessionid);
                if (!(session is Session _))
                {
                    throw new System.ArgumentException("SessionId not valid");
                }
                if (session.Owner.Id != currentuser.Id)
                {
                    throw new System.Exception($"User {currentusername} not owner of session {session.Name}");
                }
                await Db.AddUserToSession(session.Id, adduser.Id);
            }
            catch (Exception e)
            {
                Logger.LogError($"{nameof(AddUser)} failed: {e.Message}");
                return Forbid();
            }
            return new EmptyResult();
        }
    }
}