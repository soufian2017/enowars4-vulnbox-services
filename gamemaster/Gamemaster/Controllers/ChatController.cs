using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Gamemaster.Database;
using Gamemaster.Models.Database;
using Gamemaster.Models.View;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace Gamemaster.Controllers
{
    [Route("api/[controller]/[action]")]
    [ApiController]
    public class ChatController : Controller
    {
        private readonly ILogger<ChatController> Logger;
        private readonly IGamemasterDb Db;

        public ChatController(ILogger<ChatController> logger, IGamemasterDb db)
        {
            Logger = logger;
            Db = db;
        }
        [HttpPost]
        public async Task<IActionResult> GetRecent([FromForm]long id) //TODO Weird stuff
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
            var session = await Db.GetSession(id, currentuser.Id);
            if (!(session is ExtendedSessionView _))
            {
                throw new System.ArgumentException("SessionId not valid or User not in Session");
            }
            ChatMessageView[] messages = await Db.GetChatMessages(id);
            return Json(messages);
        }
    }
}