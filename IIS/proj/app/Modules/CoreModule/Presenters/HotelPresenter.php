<?php


namespace App\Modules\CoreModule\Presenters;
use Nette;
use Nette\Utils\Html;
use Ublaboo\DataGrid\DataGrid;

class HotelPresenter extends Nette\Application\UI\Presenter
{
    /**
     * @inject
     * @var Nette\Database\Context
     */
    public $database;

    // Rendering the Hotel table data in home page
    public function createComponentHotelGrid($name): DataGrid
    {
        $grid = new DataGrid($this, $name);
        $grid->setDataSource($this->database->table('hotel')->fetchAll());

        $grid->addColumnText('name','Name')
            ->setRenderer(function ($row) {
                return Html::el("a",['href' => $this->link('Room:', ['hotel_id' => $row['id']])])
                    ->setText($row['name']);
            })->setFilterText();
        $grid->addColumnText('city','City')->setFilterText();
        $grid->addColumnText('street', 'Street')
            ->setRenderer(function ($row) {
                return $row["street"];
            })
            ->setFilterText();
        $grid->addColumnText('description','Description')->setFilterText();
        $grid->addColumnText('stars','Stars')->setSortable()->setFilterText();
        $grid->addColumnText('rating','Rating')->setSortable()->setFilterText();

        return $grid;
    }
}