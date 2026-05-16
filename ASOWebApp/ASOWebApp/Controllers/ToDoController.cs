using ASOWebApp.Models;
using Microsoft.AspNetCore.Mvc;

namespace ASOWebApp.Controllers
{
    public class ToDoController : Controller
    {
        private const int MaxItemsLimit = 10;
        private static List<ToDoItem> _todoItems = new List<ToDoItem>();
        private static int _nextId = 1;
        private static CircularBuffer<string> _historyLog = new CircularBuffer<string>(MaxItemsLimit);

        public IActionResult Index()
        {
            ViewBag.History = _historyLog.GetAll().ToList();
            return View(_todoItems);
        }

        [HttpPost]
        public IActionResult Add(string title)
        {
            if (!string.IsNullOrWhiteSpace(title))
            {
                _todoItems.Add(new ToDoItem { Id = _nextId++, Title = title, IsDone = false });
                _historyLog.Enqueue($"Added task: '{title}'");

                if (_todoItems.Count > MaxItemsLimit)
                {
                    _todoItems.RemoveAt(0); // Removes the oldest task
                }
            }
            return RedirectToAction("Index");
        }

        [HttpPost]
        public IActionResult Toggle(int id)
        {
            var item = _todoItems.FirstOrDefault(t => t.Id == id);
            if (item != null)
            {
                item.IsDone = !item.IsDone;
                string status = item.IsDone ? "completed" : "uncompleted";
                _historyLog.Enqueue($"Marked '{item.Title}' as {status}");
            }
            return RedirectToAction("Index");
        }

        [HttpPost]
        public IActionResult Delete(int id)
        {
            var item = _todoItems.FirstOrDefault(t => t.Id == id);
            if (item != null)
            {
                _todoItems.Remove(item);
                _historyLog.Enqueue($"Deleted task: '{item.Title}'");
            }
            return RedirectToAction("Index");
        }

        [HttpPost]
        public IActionResult Edit(int id, string title)
        {
            var item = _todoItems.FirstOrDefault(t => t.Id == id);
            if (item != null && !string.IsNullOrWhiteSpace(title))
            {
                _historyLog.Enqueue($"Changed task '{item.Title}' to '{title}'");
                item.Title = title;
            }
            return RedirectToAction("Index"); 
        }
    }
}
