<?php

namespace App\Modules\CoreModule\Presenters;
use Nette;
use Nette\Utils\Html;
use Ublaboo\DataGrid\DataGrid;

class RoomPresenter extends Nette\Application\UI\Presenter
{
    /**
     * @inject
     * @var Nette\Database\Context
     */
    public $database;

    /** @persistent */
    public $hotel_id;

    public function renderDefault(int $hotel_id): void
    {
        if (!isset($this->hotel_id))
            $this->hotel_id = $hotel_id;
        $this->template->hotel =
            $this->database->table('hotel')->where('id', $hotel_id)->fetch();

        $this->template->room_types =
            $this->database->table('room_details')->where('hotel_id', $hotel_id);
    }

    public function createComponentRoomGrid(string $name): DataGrid
    {
        $grid = new DataGrid($this, $name);
        $grid->setPrimaryKey('type');
        $grid->setDataSource
            ($this->database->table('room_details')->where('hotel_id', $this->hotel_id));

        $grid->addColumnText('type','Type')->setRenderer(function ($row) {
            return Html::el("a",['href' => $this->link('Reservation:',
                ['hotel_id' => $this->hotel_id, 'type' => $row['type']])])->setText($row['type']);
        })->setFilterText();
        $grid->addColumnText('beds','Beds')->setSortable()->setFilterRange();
        $grid->addColumnText('facility','Facility');
        $grid->addColumnText('price','Price')->setSortable()->setFilterRange();

        //$grid->setDefaultSort(['type' => 'DESC']);
        return $grid;
    }
}